#ifndef CRYPTO_H
#define CRYPTO_H

#include <QByteArray>
#include <QString>

namespace Crypto
{
    const QString GenericAESKey = "a3K8Bx%2r8Y7#xDh";

    QByteArray decryptPack(const QByteArray& packBase64, const QString& key);
    QByteArray encryptPack(const QByteArray& pack, const QString& key);
    void addPKCS7Padding(QByteArray& packBase64);
}   // namespace Crypto

#endif   // CRYPTO_H
