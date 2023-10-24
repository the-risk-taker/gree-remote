#include "deviceitem.h"
#ifdef Q_OS_MAC
#include "mac/macnativelabel.h"
#endif

#include <QDebug>
#include <QEvent>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>

DeviceItem::DeviceItem(const QPointer<Device>& device, QObject* parent) : QWidgetAction(parent), m_device(device)
{
    //    installEventFilter(this);

    //    startTimer(100);
}

bool DeviceItem::event(QEvent* e)
{
    qInfo() << "DeviceItem::event:" << e->type();

    //    if (e->type() == QEvent::Timer)
    //        m_widget->update();

    return QWidgetAction::event(e);
}

bool DeviceItem::eventFilter(QObject* o, QEvent* e)
{
    qInfo() << "DeviceItem::eventFilter::event:" << e->type();

    return QWidgetAction::eventFilter(o, e);
}
