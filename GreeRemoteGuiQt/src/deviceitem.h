#ifndef DEVICEITEM_H
#define DEVICEITEM_H

#include <QPalette>
#include <QPointer>
#include <QWidgetAction>

class Device;

class DeviceItem : public QWidgetAction
{
  public:
    explicit DeviceItem(const QPointer<Device>& device, QObject* parent = nullptr);

  protected:
    bool event(QEvent* e) override;
    bool eventFilter(QObject* o, QEvent* e) override;

  private:
    const QPointer<Device> m_device;
};

#endif   // DEVICEITEM_H
