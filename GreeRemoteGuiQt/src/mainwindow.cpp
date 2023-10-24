#include "mainwindow.h"
#include "device.h"
#include "devicefinder.h"
#include "deviceitem.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopServices>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QUrl>
#include <QVBoxLayout>

MainWindow::MainWindow(DeviceFinder& deviceFinder, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_trayIcon(new QSystemTrayIcon()),
    m_deviceFinder(deviceFinder)
{
    ui->setupUi(this);

    connect(ui->scanButton, &QPushButton::clicked, this, &MainWindow::onScanButtonClicked);
    connect(ui->deviceComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::onComboBoxIndexChanged);
    connect(ui->powerOnCheckBox, &QCheckBox::clicked, this, &MainWindow::onPowerCheckBoxClicked);
    connect(ui->healthModeCheckBox, &QCheckBox::clicked, this, &MainWindow::onHealthModeCheckBoxClicked);
    connect(ui->turboModeCheckBox, &QCheckBox::clicked, this, &MainWindow::onTurboModeCheckBoxClicked);
    connect(ui->quietModeCheckBox, &QCheckBox::clicked, this, &MainWindow::onQuietModeCheckBoxClicked);
    connect(ui->lightCheckBox, &QCheckBox::clicked, this, &MainWindow::onLightCheckBoxClicked);
    connect(ui->modeComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, &MainWindow::onModeComboBoxActivated);
    connect(ui->fanSpeedComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, &MainWindow::onFanSpeedComboBoxActivated);
    connect(ui->verticalSwingModeComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, &MainWindow::onVerticalSwingModeComboBoxActivated);
    connect(ui->setTempButton, &QPushButton::clicked, this, &MainWindow::onSetTempButtonClicked);
    connect(ui->xfanCheckBox, &QCheckBox::clicked, this, &MainWindow::onXfanCheckBoxClicked);
    connect(ui->airModeCheckBox, &QCheckBox::clicked, this, &MainWindow::onAirModeCheckBoxClicked);
    connect(ui->sleepModeCheckBox, &QCheckBox::clicked, this, &MainWindow::onSleepModeCheckBoxClicked);
    connect(ui->savingModeCheckBox, &QCheckBox::clicked, this, &MainWindow::onSavingModeCheckBoxClicked);

    connect(ui->flaticonCreditsLabel, &QLabel::linkActivated, this, &MainWindow::onLabelLinkClicked);

    QIcon icon {":/icon-v2"};

    // Mask makes possible to render the icon properly on Dark menu bars
    icon.setIsMask(true);

    m_trayIcon->setIcon(icon);
    m_trayIcon->show();

    m_scanAction = new QAction("Scan devices");
    connect(m_scanAction, &QAction::triggered, [this] {
        emit scanInitiated();
        m_scanAction->setEnabled(false);
    });

    auto trayMenu = new QMenu {this};
    trayMenu->addAction(m_scanAction);

    trayMenu->addAction("Preferences...", [this] { show(); });

    trayMenu->addSeparator();
    m_separatorAction = trayMenu->addSeparator();
    trayMenu->addAction("Quit", [] { qApp->exit(); });

    m_trayIcon->setContextMenu(trayMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addDevice(const QPointer<Device>& device)
{
    qInfo() << "device added:" << device->descritptor().id;

    createDeviceMenuItem(device);
    createDeviceTableEntry(device);
}

void MainWindow::onScanFinished()
{
    m_scanAction->setEnabled(true);
    ui->scanButton->setEnabled(true);
}

void MainWindow::onBindingFinished() {}

void MainWindow::createDeviceMenuItem(const QPointer<Device>& device)
{
    if (!device)
        return;

    auto&& contextMenu = m_trayIcon->contextMenu();
    //    contextMenu->addSeparator();

    auto&& deviceMenu = new QMenu(contextMenu);
    deviceMenu->setTitle(device->descritptor().name);
    contextMenu->insertMenu(m_separatorAction, deviceMenu);

    auto powerAction = new QAction(deviceMenu);
    powerAction->setText("Power: Off");
    powerAction->setCheckable(true);
    connect(powerAction, &QAction::toggled, [powerAction] { powerAction->setText(powerAction->isChecked() ? "Power: On" : "Power: Off"); });

    deviceMenu->addAction(powerAction);

    auto modeAction = new QAction(deviceMenu);
    modeAction->setText("Mode: fan");
    auto modeMenu = new QMenu(deviceMenu);
    modeMenu->addAction("Fan", [modeAction] { modeAction->setText("Mode: fan"); });
    modeMenu->addAction("Cool", [modeAction] { modeAction->setText("Mode: cool"); });
    modeMenu->addAction("Heat", [modeAction] { modeAction->setText("Mode: heat"); });
    modeMenu->addAction("Dry", [modeAction] { modeAction->setText("Mode: dry"); });
    modeAction->setMenu(modeMenu);
    deviceMenu->addAction(modeAction);

    //    auto w = new QWidget(contextMenu);
    //    auto l = new QVBoxLayout;

    //    l->setSpacing(0);
    //    l->setContentsMargins(15, 4, 4, 4);

    //    w->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    //    w->setLayout(l);

    //    auto buttonLayout = new QHBoxLayout;
    //    buttonLayout->setContentsMargins(0, 0, 0, 0);
    //    buttonLayout->addWidget(new QPushButton("Up", w));
    //    buttonLayout->addWidget(new QPushButton("Down", w));
    //    l->addLayout(buttonLayout);

    //    auto action = new QWidgetAction(contextMenu);
    //    action->setDefaultWidget(w);

    auto action = new DeviceItem(device, contextMenu);
    deviceMenu->addAction(action);
    // contextMenu->installEventFilter(action);
}

void MainWindow::onLabelLinkClicked(const QString& link)
{
    QDesktopServices::openUrl({link});
}

void MainWindow::createDeviceTableEntry(const QPointer<Device>& device)
{
    auto t = ui->deviceTable;
    auto row = qMax(0, t->rowCount() - 1);

    t->insertRow(row);
    t->setItem(row, 0, new QTableWidgetItem(device->descritptor().id));
    t->setItem(row, 1, new QTableWidgetItem(device->descritptor().name));
    t->setItem(row, 2, new QTableWidgetItem(device->descritptor().address.toString()));
    t->setItem(row, 3, new QTableWidgetItem(device->descritptor().key));

    ui->deviceComboBox->addItem(device->descritptor().name, device->descritptor().id);
}

void MainWindow::selectTestDevice(const QString& id)
{
    qInfo() << "Selecting test device:" << id;

    auto&& device = m_deviceFinder.getDeviceById(id);
    if (device.isNull())
    {
        qWarning() << "Failed to obtain device";
        m_selectedDevice.clear();
        return;
    }

    if (!m_selectedDevice.isNull())
        m_selectedDevice->disconnect(this);

    connect(device, &Device::statusUpdated, this, &MainWindow::updateTestDeviceStatus);

    m_selectedDevice = device;

    updateTestDeviceStatus();
}

void MainWindow::updateTestDeviceStatus()
{
    if (m_selectedDevice.isNull())
        return;

    // Status display
    ui->powerStateLabel->setText(m_selectedDevice->isPoweredOn() ? "On" : "Off");
    ui->healthStatusLabel->setText(m_selectedDevice->isHealthEnabled() ? "On" : "Off");
    ui->turboModeStatusLabel->setText(m_selectedDevice->isTurboEnabled() ? "On" : "Off");
    ui->quietModeStatusLabel->setText(m_selectedDevice->isQuietModeEnabled() ? "On" : "Off");
    ui->lightStatusLabel->setText(m_selectedDevice->isLightEnabled() ? "On" : "Off");
    ui->modeLabel->setText(QString::number(m_selectedDevice->mode()));
    ui->temperatureLabel->setText(QString::number(m_selectedDevice->temperature()));
    ui->fanSpeedLabel->setText(QString::number(m_selectedDevice->fanSpeed()));
    ui->horizontalSwingLabel->setText(QString::number(m_selectedDevice->horizontalSwingMode()));
    ui->verticalSwingLabel->setText(QString::number(m_selectedDevice->verticalSwingMode()));
    ui->xfanStatusLabel->setText(m_selectedDevice->isXfanBlowEnabled() ? "On" : "Off");
    ui->airModeStatusLabel->setText(m_selectedDevice->isAirModeEnabled() ? "On" : "Off");
    ui->sleepModeStatusLabel->setText(m_selectedDevice->isSleepModeEnabled() ? "On" : "Off");
    ui->savingModeStatusLabel->setText(m_selectedDevice->isSavingModeEnabled() ? "On" : "Off");

    // Device control
    ui->modeComboBox->setCurrentIndex(m_selectedDevice->mode());
    ui->temperatureSpinBox->setValue(m_selectedDevice->temperature());
    ui->verticalSwingModeComboBox->setCurrentIndex(m_selectedDevice->verticalSwingMode());
    ui->healthModeCheckBox->setChecked(m_selectedDevice->isHealthEnabled());
    ui->turboModeCheckBox->setChecked(m_selectedDevice->isTurboEnabled());
    ui->quietModeCheckBox->setChecked(m_selectedDevice->isQuietModeEnabled());
    ui->lightCheckBox->setChecked(m_selectedDevice->isLightEnabled());
    ui->powerOnCheckBox->setChecked(m_selectedDevice->isPoweredOn());
    ui->fanSpeedComboBox->setCurrentIndex(m_selectedDevice->fanSpeed());
    ui->xfanCheckBox->setChecked(m_selectedDevice->isXfanBlowEnabled());
    ui->airModeCheckBox->setChecked(m_selectedDevice->isAirModeEnabled());
    ui->sleepModeCheckBox->setChecked(m_selectedDevice->isSleepModeEnabled());
    ui->savingModeCheckBox->setChecked(m_selectedDevice->isSavingModeEnabled());
}

void MainWindow::onScanButtonClicked()
{
    emit scanInitiated();
    ui->scanButton->setEnabled(false);
}

void MainWindow::onComboBoxIndexChanged(int)
{
    selectTestDevice(ui->deviceComboBox->currentData().toString());
}

void MainWindow::onPowerCheckBoxClicked()
{
    if (m_selectedDevice)
        m_selectedDevice->setPoweredOn(ui->powerOnCheckBox->isChecked());
}

void MainWindow::onHealthModeCheckBoxClicked()
{
    if (m_selectedDevice)
        m_selectedDevice->setHealthEnabled(ui->healthModeCheckBox->isChecked());
}

void MainWindow::onTurboModeCheckBoxClicked()
{
    if (m_selectedDevice)
        m_selectedDevice->setTurboEnabled(ui->turboModeCheckBox->isChecked());
}

void MainWindow::onQuietModeCheckBoxClicked()
{
    if (m_selectedDevice)
        m_selectedDevice->setQuietModeEnabled(ui->quietModeCheckBox->isChecked());
}

void MainWindow::onLightCheckBoxClicked()
{
    if (m_selectedDevice)
        m_selectedDevice->setLightEnabled(ui->lightCheckBox->isChecked());
}

void MainWindow::onModeComboBoxActivated(int index)
{
    if (m_selectedDevice)
        m_selectedDevice->setMode(index);
}

void MainWindow::onFanSpeedComboBoxActivated(int index)
{
    if (m_selectedDevice)
        m_selectedDevice->setFanSpeed(index);
}

void MainWindow::onVerticalSwingModeComboBoxActivated(int index)
{
    if (m_selectedDevice)
        m_selectedDevice->setVerticalSwingMode(index);
}

void MainWindow::onSetTempButtonClicked()
{
    if (m_selectedDevice)
        m_selectedDevice->setTemperature(ui->temperatureSpinBox->value());
}

void MainWindow::onXfanCheckBoxClicked()
{
    if (m_selectedDevice)
        m_selectedDevice->setXfanBlowEnabled(ui->xfanCheckBox->isChecked());
}

void MainWindow::onAirModeCheckBoxClicked()
{
    if (m_selectedDevice)
        m_selectedDevice->setAirModeEnabled(ui->airModeCheckBox->isChecked());
}

void MainWindow::onSleepModeCheckBoxClicked()
{
    if (m_selectedDevice)
        m_selectedDevice->setSleepModeEnabled(ui->sleepModeCheckBox->isChecked());
}

void MainWindow::onSavingModeCheckBoxClicked()
{
    if (m_selectedDevice)
        m_selectedDevice->setSavingModeEnabled(ui->savingModeCheckBox->isChecked());
}
