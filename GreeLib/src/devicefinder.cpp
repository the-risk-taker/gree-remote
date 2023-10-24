#include "devicefinder.h"
#include "crypto.h"
#include "protocolutils.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QTimer>
#include <QUdpSocket>
#include <stdexcept>

DeviceFinder::DeviceFinder(QObject* parent) : QObject(parent), m_socket(new QUdpSocket(this)), m_timer(new QTimer(this))
{
    qDebug() << "initializing";

    connect(m_socket, &QUdpSocket::readyRead, this, &DeviceFinder::socketReadyRead);

    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this, &DeviceFinder::timerTimeout);
}

void DeviceFinder::scan()
{
    if (m_state != State::Idle)
    {
        qWarning() << "scanning can only be initiated in Idle state";
        return;
    }

    m_state = State::Scanning;

    qDebug() << "scanning started";

    if (!m_socket->isOpen())
    {
        qDebug() << "opening UDP socket";
        m_socket->open(QIODevice::ReadWrite);
    }

    auto written = m_socket->writeDatagram(R"({"t":"scan"})", QHostAddress {"192.168.1.255"}, 7000);
    qDebug() << "written datagram length:" << written;

    m_timer->start(2000);
}

QPointer<Device> DeviceFinder::getDevice(const DeviceDescriptor& descriptor)
{
    return getDeviceById(descriptor.id);
}

QPointer<Device> DeviceFinder::getDeviceById(const QString& id)
{
    try
    {
        return m_devices.at(id);
    }
    catch (std::out_of_range&)
    {
        auto&& existing = std::find_if(m_descriptors.cbegin(), m_descriptors.cend(), [&id](const DeviceDescriptor& d) { return id == d.id; });

        if (existing == m_descriptors.cend())
        {
            qWarning() << "no descriptor found for the device ID" << id;
            return {};
        }

        auto&& device = new Device(*existing, this);
        m_devices[id] = device;
        return device;
    }
}

void DeviceFinder::socketReadyRead()
{
    qDebug() << "socket ready read";

    std::array<char, 65536> datagram = {0};
    QHostAddress remoteAddress;
    uint16_t remotePort = 0;
    auto length = m_socket->readDatagram(datagram.data(), sizeof(datagram), &remoteAddress, &remotePort);
    qDebug() << "received datagram from" << remoteAddress << ":" << remotePort << ", length:" << length;

    if (m_state == State::Scanning)
    {
        qDebug() << "processing scan results";
        processScanResponse(QByteArray(datagram.data(), length), remoteAddress, remotePort);
        m_timer->start();
    }
    else if (m_state == State::Binding)
    {
        qDebug() << "processing bind results";
        processBindResponse(QByteArray(datagram.data(), length));
        m_timer->start();
    }
}

void DeviceFinder::timerTimeout()
{
    qDebug() << "timer timeout";

    if (m_state == State::Scanning)
    {
        qDebug() << "scanning finished";

        m_state = State::Idle;
        emit scanFinshed();
        bindDevices();
    }
    else if (m_state == State::Binding)
    {
        qDebug() << "binding finished";

        m_state = State::Idle;
        emit bindingFinished();
    }
    else
    {
        qWarning() << "timer timeout in Idle state";
    }
}

void DeviceFinder::processScanResponse(const QByteArray& response, const QHostAddress& remoteAddress, uint16_t remotePort)
{
    qDebug() << "processing scan response" << response;

    QJsonObject pack;
    if (!ProtocolUtils::readPackFromResponse(response, Crypto::GenericAESKey, pack))
    {
        qWarning() << "failed to read pack from response";
        return;
    }

    auto&& id = pack["cid"].toString();

    auto&& existing = std::find_if(m_descriptors.cbegin(), m_descriptors.cend(), [&id](const DeviceDescriptor& descriptor) { return descriptor.id == id; });

    if (existing != m_descriptors.cend())
    {
        qDebug() << "device already added:" << id;
        return;
    }

    DeviceDescriptor device;
    device.id = id;
    device.name = pack["name"].toString();
    device.address = remoteAddress;
    device.port = remotePort;

    m_descriptors.push_back(device);
}

void DeviceFinder::bindDevices()
{
    if (m_state != State::Idle)
    {
        qWarning() << "binding can only be initiated in Idle state";
        return;
    }

    m_state = State::Binding;

    bool hasPending = false;

    std::for_each(m_descriptors.cbegin(), m_descriptors.cend(), [this, &hasPending](const DeviceDescriptor& device) {
        // Check if device is already bound
        if (device.bound)
            return;

        hasPending = true;

        if (!m_socket->isOpen())
            m_socket->open(QIODevice::ReadWrite);

        auto&& bindingPacket = ProtocolUtils::createBindingRequest(device);
        auto&& encryptedBindingPacket = Crypto::encryptPack(bindingPacket, Crypto::GenericAESKey);
        auto&& request = ProtocolUtils::createDeviceRequest(encryptedBindingPacket, 1);

        qDebug() << "sending bind request to" << device.address << ":" << device.port << ":" << request;

        m_socket->writeDatagram(request, device.address, device.port);
    });

    if (hasPending)
    {
        m_timer->start();
    }
    else
    {
        // end bind state - nothing new to bind.
        qDebug() << "binding finished without new devices found";

        m_state = State::Idle;
    }
}

void DeviceFinder::processBindResponse(const QByteArray& response)
{
    qDebug() << "processing bind response:" << response;

    QJsonObject pack;
    if (!ProtocolUtils::readPackFromResponse(response, Crypto::GenericAESKey, pack))
    {
        qWarning() << "failed to read pack from response";
        return;
    }

    qDebug() << "bind response JSON:" << pack;

    auto&& key = pack["key"].toString();
    auto&& mac = pack["mac"].toString();

    if (key.isEmpty() || mac.isEmpty())
    {
        qWarning() << "binding failed, response misses manadtory fields";
        return;
    }

    auto&& device = std::find_if(m_descriptors.begin(), m_descriptors.end(), [&mac](const DeviceDescriptor& dev) { return dev.id == mac; });

    if (device == m_descriptors.end())
    {
        qWarning() << "no device found for this binding response";
        return;
    }

    device->key = key;
    device->bound = true;

    qDebug() << "device bound:" << mac;

    emit(deviceBound(*device));
}
