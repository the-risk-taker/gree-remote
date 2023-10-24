#ifndef PROTOCOLUTILS_H
#define PROTOCOLUTILS_H

#include "devicedescriptor.h"
#include <QByteArray>
#include <QMap>

namespace ProtocolUtils
{
    using DeviceParameterMap = QMap<QString, int>;

    QByteArray createBindingRequest(const DeviceDescriptor& device);
    QByteArray createDeviceRequest(const QByteArray& encryptedPack, int i = 0);
    QByteArray createDeviceStatusRequestPack(const QString& id);
    QByteArray createDeviceCommandPack(const DeviceParameterMap& parameters);

    bool readPackFromResponse(const QByteArray& response, const QString& decryptionKey, QJsonObject& pack);

    DeviceParameterMap readStatusMapFromPack(const QJsonObject& pack);
}   // namespace ProtocolUtils

#endif   // PROTOCOLUTILS_H
