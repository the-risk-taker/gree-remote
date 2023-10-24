#ifndef DEVICE_H
#define DEVICE_H

#include "devicedescriptor.h"
#include <QMap>
#include <QObject>

class QUdpSocket;
class QTimer;

class Device : public QObject
{
    Q_OBJECT

  public:
    explicit Device(const DeviceDescriptor& descriptor, QObject* parent = nullptr);
    ~Device() override;

    [[nodiscard]] const DeviceDescriptor& descritptor() const
    {
        return m_device;
    }

    void updateStatus();

    [[nodiscard]] bool isPoweredOn() const
    {
        return m_powered;
    }
    void setPoweredOn(bool on);

    // Turns indicators on the indoor unit on or off.
    [[nodiscard]] bool isLightEnabled() const
    {
        return m_light;
    }
    void setLightEnabled(bool enabled);

    // Decreases fan speed to the minimum. Not available in Dry and Fan modes.
    [[nodiscard]] bool isQuietModeEnabled() const
    {
        return m_quiet;
    }
    void setQuietModeEnabled(bool enabled);

    // "Cold Plasma" mode, only for devices with "anion generator".
    // Absorbs dust and kills bacteria.
    [[nodiscard]] bool isHealthEnabled() const
    {
        return m_health;
    }
    void setHealthEnabled(bool enabled);

    // Turbo mode sets fan speed to the maximum in Cool or Heat mode.
    // Setting fan speed while this mode is enabled will be ignored.
    [[nodiscard]] bool isTurboEnabled() const
    {
        return m_turbo;
    }
    void setTurboEnabled(bool enabled);

    // X-Fan or Blow function keeps the fan running for a while
    // after the unit has been turned off. Effective in Dry and Cool mode.
    [[nodiscard]] bool isXfanBlowEnabled() const
    {
        return m_xfanEnabled;
    }
    void setXfanBlowEnabled(bool enabled);

    // Controls the fresh air valve (if the unit has one)
    [[nodiscard]] bool isAirModeEnabled() const
    {
        return m_airModeEnabled;
    }
    void setAirModeEnabled(bool enabled);

    // Available in Cool, Heat and Dry mode.
    [[nodiscard]] bool isSleepModeEnabled() const
    {
        return m_sleepModeEnabled;
    }
    void setSleepModeEnabled(bool enabled);

    // Energy saving function locks fan speed and temperature. Inactive if Sleep Mode is enabled.
    [[nodiscard]] bool isSavingModeEnabled() const
    {
        return m_savingModeEnabled;
    }
    void setSavingModeEnabled(bool enabled);

    // Auto: 0, Cool: 1, Dry: 2, Fan: 3, Heat: 4
    [[nodiscard]] int mode() const
    {
        return m_mode;
    }
    void setMode(int mode);

    // In Celsius
    [[nodiscard]] int temperature() const
    {
        return m_temperature;
    }
    void setTemperature(int temperature);

    // Auto: 0, 1-5 (or 1-3-5 for 3-speed)
    [[nodiscard]] int fanSpeed() const
    {
        return m_fanSpeed;
    }
    void setFanSpeed(int speed);

    // Default: 0, Full swipe: 1,
    // Fix, from the uppermost position: 1/5: 2, 2/5: 3, 3/5: 4, 4/5: 5, 5/5: 6
    // Swing, from the uppermost region: 1/3: 11, 2/3: 9, 3/3: 7 (probably there are more steps, maybe 5, for 8 and 10)
    [[nodiscard]] int verticalSwingMode() const
    {
        return m_verticalSwingMode;
    }
    void setVerticalSwingMode(int mode);

    // Default: 0
    [[nodiscard]] int horizontalSwingMode() const
    {
        return m_horizontalSwingMode;
    }
    void setHorizontalSwingMode(int mode);

    using ParameterMap = QMap<QString, int>;
    void setParameters(const ParameterMap& parameters);

  signals:
    void statusUpdated();

  private:
    enum class State
    {
        Unbound,
        Idle,
        StatusUpdate,
        Command
    };

    State m_state = State::Idle;
    DeviceDescriptor m_device;
    QUdpSocket* m_socket;
    QTimer* m_pollTimer;

    bool m_powered = false;
    bool m_health = false;
    bool m_turbo = false;
    bool m_light = false;
    bool m_quiet = false;
    bool m_xfanEnabled = false;
    bool m_airModeEnabled = false;
    bool m_sleepModeEnabled = false;
    bool m_savingModeEnabled = false;
    int m_mode = 0;
    int m_temperature = 0;
    int m_fanSpeed = 0;
    int m_verticalSwingMode = 0;
    int m_horizontalSwingMode = 0;

    void openSocket();

    void onPollTimerTimeout();
    void onSocketReadyRead();
    void deviceRequest(const QByteArray& request);

    void processStatusUpdateResponse(const QByteArray& response);
    void processCommandResponse(const QByteArray& response);

    void updateFanParameters();
};

#endif   // DEVICE_H
