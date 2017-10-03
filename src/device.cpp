#include "device.h"
#include "crypto.h"
#include "protocolutils.h"

#include <QLoggingCategory>
#include <QNetworkDatagram>
#include <QUdpSocket>
#include <QTimer>

Q_DECLARE_LOGGING_CATEGORY(DeviceLog)
Q_LOGGING_CATEGORY(DeviceLog, "Device")

Device::Device(const DeviceDescriptor &descriptor, QObject *parent)
    : QObject(parent)
    , m_device(descriptor)
    , m_socket(new QUdpSocket(this))
    , m_pollTimer(new QTimer(this))
{
    connect(m_socket, &QUdpSocket::readyRead, this, &Device::onSocketReadyRead);
    connect(m_pollTimer, &QTimer::timeout, this, &Device::onPollTimerTimeout);

    qCInfo(DeviceLog) << "device controller created for" << descriptor.name << "(" << descriptor.id << ")";

    m_pollTimer->start(5000);
}

Device::~Device()
{
    if (m_socket->isOpen())
        m_socket->close();
}

void Device::deviceRequest(const QByteArray& request)
{
    openSocket();
    auto written = m_socket->writeDatagram(request, m_device.address, m_device.port);
    qCDebug(DeviceLog) << m_device.id << "sending request datagram. Written bytes:" << written;
}

void Device::processStatusUpdateResponse(const QByteArray &response)
{
    qCDebug(DeviceLog) << "processing status update response:" << response;
}

void Device::updateStatus()
{
    auto&& pack = ProtocolUtils::createDeviceStatusRequestPack(m_device.id);
    auto&& encryptedPack = Crypto::encryptPack(pack, m_device.key);
    auto&& request = ProtocolUtils::createDeviceRequest(encryptedPack, 0);

    m_state = State::StatusUpdate;

    deviceRequest(request);
}

void Device::openSocket()
{
    if (m_socket->isOpen())
        return;

    qCDebug(DeviceLog) << m_device.id << "opening socket";
    m_socket->open(QIODevice::ReadWrite);

    qCDebug(DeviceLog) << m_device.id << "binding to" << m_device.address << ":" << m_device.port;
    if (!m_socket->bind(m_device.address, m_device.port, QUdpSocket::ShareAddress))
    {
        qCWarning(DeviceLog) << m_device.id << "binding failed. Error:" << m_socket->errorString();
        return;
    }

    m_state = State::Idle;
}

void Device::onPollTimerTimeout()
{
    qCDebug(DeviceLog) << m_device.id << "poll timer timeout";

    updateStatus();
}

void Device::onSocketReadyRead()
{
    qCDebug(DeviceLog) << m_device.id << "socket ready read";

    auto&& datagram = m_socket->receiveDatagram();
    qCDebug(DeviceLog) << "received datagram from" << datagram.senderAddress() << ":" << datagram.senderPort();

    if (m_state == State::StatusUpdate)
    {
        processStatusUpdateResponse(datagram.data());
        m_state = State::Idle;
    }
}
