package tomikaa.greeremote.Gree.Device;

/**
 * Created by tomikaa on 2017. 11. 27..
 */

public interface Device {

    enum Mode {
        AUTO,
        COOL,
        DRY,
        FAN,
        HEAT
    }

    enum FanSpeed {
        AUTO,
        LOW,
        MEDIUM_LOW,
        MEDIUM,
        MEDIUM_HIGH,
        HIGH
    }

    enum TemperatureUnit {
        CELSIUS,
        FAHRENHEIT
    }

    enum VerticalSwingMode {
        DEFAULT,
        FULL,
        FIXED_TOP,
        FIXED_MIDDLE_TOP,
        FIXED_MIDDLE,
        FIXED_MIDDLE_BOTTOM,
        FIXED_BOTTOM,
        SWING_BOTTOM,
        SWING_MIDDLE_BOTTOM,
        SWING_MIDDLE,
        SWING_MIDDLE_TOP,
        SWING_TOP
    }

    String getId();

    String getName();
    void setName(String name);

    Mode getMode();
    void setMode(Mode mode);

    FanSpeed getFanSpeed();
    void setFanSpeed(FanSpeed fanSpeed);

    int getTemperature();
    void setTemperature(int value, TemperatureUnit unit);

    boolean isPoweredOn();
    void setPoweredOn(boolean poweredOn);

    boolean isLightEnabled();
    void setLightEnabled(boolean enabled);

    boolean isQuietModeEnabled();
    void setQuietModeEnabled(boolean enabled);

    boolean isTurboModeEnabled();
    void setTurboModeEnabled(boolean enabled);

    boolean isHealthModeEnabled();
    void setHealthModeEnabled(boolean enabled);

    boolean isAirModeEnabled();
    void setAirModeEnabled(boolean enabled);

    boolean isXfanModeEnabled();
    void setXfanModeEnabled(boolean enabled);

    boolean isSavingModeEnabled();
    void setSavingModeEnabled(boolean enabled);

    boolean isSleepModeEnabled();
    void setSleepModeEnabled(boolean enabled);

    VerticalSwingMode getVerticalSwingMode();
    void setVerticalSwingMode(VerticalSwingMode mode);

    int getParameter(String name);
    void setParameter(String name, int value);
}
