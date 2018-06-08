#include "mainwindow.h"

#define TITLE "Walkie Talkie Demo"

QPlainTextEdit *debug_edit = nullptr;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(type)
    Q_UNUSED(context)

    QMetaObject::invokeMethod(debug_edit, "appendPlainText", Q_ARG(QString, msg));
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(TITLE);

    m_audio_lib = nullptr;
    m_discover_instance = new AudioStreamingLibCore(this);

    listwidgetpeers = new QListWidget(this);

    connect(listwidgetpeers, &QListWidget::itemClicked, this, &MainWindow::selectPeer);

    m_audio_recorder = nullptr;

    m_paused = true;

    //Common

    comboboxaudioinput = new QComboBox(this);
    comboboxaudiooutput = new QComboBox(this);

    comboboxaudioinput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    comboboxaudiooutput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    //WebClient

    buttonconnecttopeer = new QPushButton(this);
    buttonconnecttopeer->setText("Connect to peer");
    buttonconnecttopeer->setEnabled(false);

    labelservertweb = new QLabel(this);
    labelservertweb->setText("Server:");

    lineserverweb = new QLineEdit(this);
    lineserverweb->setText("127.0.0.1");

    labelportweb = new QLabel(this);
    labelportweb->setText("Port:");

    lineportweb = new QLineEdit(this);
    lineportweb->setMinimumWidth(100);
    lineportweb->setText("1024");

    buttonstartweb = new QPushButton(this);
    buttonstartweb->setText("Connect to server");

    labelwebid = new QLabel(this);
    labelwebid->setText("ID:");

    linewebid = new QLineEdit(this);

    labelwebpassword = new QLabel(this);
    labelwebpassword->setText("Password:");

    linewebpassword = new QLineEdit(this);
    linewebpassword->setEchoMode(QLineEdit::Password);

    labelsamplerateweb = new QLabel(this);
    labelsamplerateweb->setText("Sample rate:");
    linesamplerateweb = new QLineEdit(this);
    linesamplerateweb->setText("44100");

    labelchannelsweb = new QLabel(this);
    labelchannelsweb->setText("Channels:");
    linechannelsweb = new QLineEdit(this);
    linechannelsweb->setText("1");

    labelbuffertimeweb = new QLabel(this);
    labelbuffertimeweb->setText("Buffer time (ms):");
    linebuffertimeweb = new QLineEdit(this);
    linebuffertimeweb->setText("300");

    widget1 = new QWidget(this);

    {
        QGridLayout *layout = new QGridLayout(widget1);

        layout->addWidget(buttonconnecttopeer, 0, 0, 1, 2);
        layout->addWidget(labelservertweb, 1, 0);
        layout->addWidget(lineserverweb, 1, 1);
        layout->addWidget(labelportweb, 2, 0);
        QGridLayout *layout1 = new QGridLayout();
        layout1->addWidget(lineportweb, 1, 0);
        layout1->addWidget(buttonstartweb, 1, 1);
        layout->addLayout(layout1, 2, 1);
        layout->addWidget(labelwebid, 3, 0);
        layout->addWidget(linewebid, 3, 1);
        layout->addWidget(labelwebpassword, 4, 0);
        layout->addWidget(linewebpassword, 4, 1);
        layout->addWidget(labelsamplerateweb, 5, 0);
        layout->addWidget(linesamplerateweb, 5, 1);
        layout->addWidget(labelchannelsweb, 6, 0);
        layout->addWidget(linechannelsweb, 6, 1);
        layout->addWidget(labelbuffertimeweb, 7, 0);
        layout->addWidget(linebuffertimeweb, 7, 1);
    }

    //Client

    labelhost = new QLabel(this);
    linehost = new QLineEdit(this);
    linehost->setMinimumWidth(100);
    labelhost->setText("Host:");
    linehost->setText("localhost");

    labelport = new QLabel(this);
    lineport = new QLineEdit(this);
    lineport->setMinimumWidth(100);
    labelport->setText("Port:");
    lineport->setText("1024");

    texteditsettings = new QPlainTextEdit(this);
    texteditsettings->setReadOnly(true);

    buttonconnect = new QPushButton(this);
    buttonconnect->setText("Connect");

    labelclientid = new QLabel(this);
    labelclientid->setText("ID:");

    lineclientid = new QLineEdit(this);

    labelclientpassword = new QLabel(this);
    labelclientpassword->setText("Password:");

    lineclientpassword = new QLineEdit(this);
    lineclientpassword->setEchoMode(QLineEdit::Password);

    widget2 = new QWidget(this);

    {
        QGridLayout *layout = new QGridLayout(widget2);

        layout->addWidget(labelhost, 0, 0, 1, 1);
        layout->addWidget(linehost, 0, 1, 1, 2);
        layout->addWidget(labelport, 1, 0, 1, 1);
        layout->addWidget(lineport, 1, 1, 1, 1);
        layout->addWidget(buttonconnect, 1, 2, 1, 1);
        layout->addWidget(labelclientid, 2, 0, 1, 1);
        layout->addWidget(lineclientid, 2, 1, 1, 2);
        layout->addWidget(labelclientpassword, 3, 0, 1, 1);
        layout->addWidget(lineclientpassword, 3, 1, 1, 2);
    }

    //Server

    labelportserver = new QLabel(this);
    labelportserver->setText("Port:");

    lineportserver = new QLineEdit(this);
    lineportserver->setMinimumWidth(100);
    lineportserver->setText("1024");

    buttonstartserver = new QPushButton(this);
    buttonstartserver->setText("Start Server");

    labelserverid = new QLabel(this);
    labelserverid->setText("ID:");

    lineserverid = new QLineEdit(this);

    labelserverpassword = new QLabel(this);
    labelserverpassword->setText("Password:");

    lineserverpassword = new QLineEdit(this);
    lineserverpassword->setEchoMode(QLineEdit::Password);

    labelsamplerate = new QLabel(this);
    labelsamplerate->setText("Sample rate:");
    linesamplerate = new QLineEdit(this);
    linesamplerate->setText("44100");

    labelchannels = new QLabel(this);
    labelchannels->setText("Channels:");
    linechannels = new QLineEdit(this);
    linechannels->setText("1");

    labelbuffertime = new QLabel(this);
    labelbuffertime->setText("Buffer time (ms):");
    linebuffertime = new QLineEdit(this);
    linebuffertime->setText("300");

    widget3 = new QWidget(this);

    {
        QGridLayout *layout = new QGridLayout(widget3);

        layout->addWidget(labelportserver, 0, 0);
        QGridLayout *layout1 = new QGridLayout();
        layout1->addWidget(lineportserver, 0, 0);
        layout1->addWidget(buttonstartserver, 0, 1);
        layout->addLayout(layout1, 0, 1);
        layout->addWidget(labelserverid, 1, 0);
        layout->addWidget(lineserverid, 1, 1);
        layout->addWidget(labelserverpassword, 2, 0);
        layout->addWidget(lineserverpassword, 2, 1);
        layout->addWidget(labelsamplerate, 3, 0);
        layout->addWidget(linesamplerate, 3, 1);
        layout->addWidget(labelchannels, 4, 0);
        layout->addWidget(linechannels, 4, 1);
        layout->addWidget(labelbuffertime, 5, 0);
        layout->addWidget(linebuffertime, 5, 1);
    }

    //Volume

    labelvolume = new QLabel(this);
    slidervolume = new QSlider(Qt::Horizontal, this);
    slidervolume->setRange(0, 100);

    boxinputmuted = new QCheckBox(this);
    boxinputmuted->setText("Mute input");

    tabwidget = new QTabWidget(this);

    scrollclientserver = new QScrollArea(this);

#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS) && !defined(Q_OS_WINPHONE)
    scrollclientserver->setWidgetResizable(true);
    scrollclientserver->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
#endif

    QWidget *widgetserverclient = new QWidget(this);
    QGridLayout *layoutserverclient = new QGridLayout(widgetserverclient);

    layoutserverclient->addWidget(new QLabel("Input device:", this), 0, 0);
    layoutserverclient->addWidget(comboboxaudioinput, 0, 1);
    layoutserverclient->addWidget(new QLabel("Output device:", this), 1, 0);
    layoutserverclient->addWidget(comboboxaudiooutput, 1, 1);

    {
        QWidget *widget = new QWidget(this);
        QGridLayout *layout = new QGridLayout(widget);
        layout->setMargin(0);

        QGroupBox *box1 = new QGroupBox(widget);
        box1->setTitle("Web Client");
        QGridLayout *layout1 = new QGridLayout(box1);
        layout1->addWidget(widget1, 0, 0);

        layout->addWidget(box1, 0, 0);
        layoutserverclient->addWidget(widget, 2, 0, 1, 2);
    }

    {
        QWidget *widget = new QWidget(this);
        QGridLayout *layout = new QGridLayout(widget);
        layout->setMargin(0);

        QGroupBox *box1 = new QGroupBox(widget);
        box1->setTitle("Client");
        QGridLayout *layout1 = new QGridLayout(box1);
        layout1->addWidget(widget2, 0, 0);

        layout->addWidget(box1, 0, 0);
        layoutserverclient->addWidget(widget, 3, 0, 1, 2);
    }

    {
        QWidget *widget = new QWidget(this);
        QGridLayout *layout = new QGridLayout(widget);
        layout->setMargin(0);

        QGroupBox *box1 = new QGroupBox(widget);
        box1->setTitle("Server");
        QGridLayout *layout1 = new QGridLayout(box1);
        layout1->addWidget(widget3, 0, 0);

        layout->addWidget(box1, 0, 0);
        layoutserverclient->addWidget(widget, 4, 0, 1, 2);
    }

    scrollclientserver->setWidget(widgetserverclient);

    //Chat

    widgetchat = new QWidget(this);

    texteditchat = new QPlainTextEdit(this);
    texteditchat->setReadOnly(true);

    linechat = new QLineEdit(this);

    buttonsendchat = new QPushButton(this);
    buttonsendchat->setText("Send");

    {
        QGridLayout *layout = new QGridLayout(widgetchat);
        layout->addWidget(texteditchat, 0, 0);
        QGridLayout *layout1 = new QGridLayout();
        layout1->addWidget(linechat, 0, 0);
        layout1->addWidget(buttonsendchat, 0, 1);
        layout->addLayout(layout1, 1, 0);
    }

    linerecordpath = new QLineEdit(this);
    buttonsearch = new QPushButton(this);
    buttonrecord = new QPushButton(this);
    buttonrecordstop = new QPushButton(this);
    lcdtime = new QLCDNumber(this);
    boxautostart = new QCheckBox("Auto start recording when connected", this);

    QWidget *recorder = new QWidget(this);

    QGridLayout *layout_record = new QGridLayout(recorder);
    layout_record->addWidget(new QLabel("Wave file:", this), 0, 0);
    layout_record->addWidget(linerecordpath, 0, 1);
    layout_record->addWidget(buttonsearch, 0, 2);
    layout_record->addWidget(buttonrecord, 0, 3);
    layout_record->addWidget(buttonrecordstop, 0, 4);
    layout_record->addWidget(lcdtime, 1, 0, 1, 5);
    layout_record->addWidget(boxautostart, 2, 0, 1, 5);

    texteditlog = new QPlainTextEdit(this);
    debug_edit = texteditlog;

    tabwidget->addTab(scrollclientserver, "Client Server");
    tabwidget->addTab(listwidgetpeers, "Search");
    tabwidget->addTab(widgetchat, "Chat");
    tabwidget->addTab(recorder, "Record");
    tabwidget->addTab(texteditsettings, "Settings");
    tabwidget->addTab(texteditlog, "Log");

    connect(tabwidget, &QTabWidget::currentChanged, this, &MainWindow::currentChanged);

    connect(linechat, &QLineEdit::returnPressed, this, &MainWindow::writeText);
    connect(buttonsendchat, &QPushButton::clicked, this, &MainWindow::writeText);

    connect(slidervolume, &QSlider::valueChanged, this, &MainWindow::sliderVolumeValueChanged);

    connect(buttonstartweb, &QPushButton::clicked, this, &MainWindow::webClient);
    connect(buttonconnect, &QPushButton::clicked, this, &MainWindow::client);
    connect(buttonstartserver, &QPushButton::clicked, this, &MainWindow::server);

    connect(buttonconnecttopeer, &QPushButton::clicked, this, &MainWindow::connectToPeer);

    linechat->blockSignals(true);

    buttonsendchat->setEnabled(false);
    buttonsendchat->setDefault(true);

    texteditlog->setReadOnly(true);

    resetRecordPage();

    getDevInfo();

    levelinput = new LevelWidget(this);
    leveloutput = new LevelWidget(this);

    QWidget *mainwidget = new QWidget(this);

    QGridLayout *mainlayout = new QGridLayout(mainwidget);

    mainlayout->addWidget(levelinput, 0, 0, 4, 1);
    mainlayout->addWidget(tabwidget, 0, 1, 1, 1);
    mainlayout->addWidget(boxinputmuted, 1, 1, 1, 1);
    mainlayout->addWidget(labelvolume, 2, 1, 1, 1);
    mainlayout->addWidget(slidervolume, 3, 1, 1, 1);
    mainlayout->addWidget(leveloutput, 0, 4, 4, 1);

    //Recorder
    connect(buttonsearch, &QPushButton::clicked, this, &MainWindow::setRecordPath);
    connect(buttonrecord, &QPushButton::clicked, this, &MainWindow::startPauseRecord);
    connect(buttonrecordstop, &QPushButton::clicked, this, &MainWindow::stopRecord);

    widget1->setFixedHeight(widget1->sizeHint().height());
    widget2->setFixedHeight(widget2->sizeHint().height());
    widget3->setFixedHeight(widget3->sizeHint().height());

    slidervolume->setValue(100);

    setCentralWidget(mainwidget);

    connect(linewebid, &QLineEdit::textChanged, [=](const QString &text){
        linewebid->blockSignals(true);
        linewebid->setText(cleanString(text));
        linewebid->blockSignals(false);
    });

    connect(linewebpassword, &QLineEdit::textChanged, [=](const QString &text){
        linewebpassword->blockSignals(true);
        linewebpassword->setText(cleanString(text));
        linewebpassword->blockSignals(false);
    });

    qInstallMessageHandler(messageHandler);
}

MainWindow::~MainWindow()
{
    qInstallMessageHandler(0);
}

void MainWindow::currentChanged(int index)
{
    if (index < 0)
        return;

    if (index == 1)
        startDiscover();
    else
        stopDiscover();
}

void MainWindow::startDiscover()
{
    if (!isVisible() || !listwidgetpeers->isEnabled())
        return;

    listwidgetpeers->clear();

    DiscoverClient *instance = m_discover_instance->discoverInstance();

    connect(instance, &DiscoverClient::peerFound, this, &MainWindow::peerFound);
    connect(this, &MainWindow::stoprequest, instance, &DiscoverClient::deleteLater);

    instance->discover(lineport->text().toInt(), QByteArray("WalkieTalkieTCPDemo"));
}

void MainWindow::stopDiscover()
{
    if (!isVisible())
        return;

    emit stoprequest();
    listwidgetpeers->clear();
}

void MainWindow::peerFound(const QHostAddress &address, const QString &id)
{
    QString peer;

    QString ip = QHostAddress(address.toIPv4Address()).toString();

    peer.append(QString("IP: %0").arg(ip));

    if (!id.isEmpty())
        peer.append(QString(" - ID: %0").arg(id));

    QListWidgetItem *item = new QListWidgetItem();
    item->setText(peer);
    item->setData(Qt::UserRole, ip);

    listwidgetpeers->addItem(item);
}

void MainWindow::selectPeer(QListWidgetItem *item)
{
    linehost->setText(item->data(Qt::UserRole).toString());
    linehost->setFocus();
    tabwidget->setCurrentIndex(0);
}

void MainWindow::webClient()
{
    if (m_audio_lib)
    {
        buttonrecord->setEnabled(false);

        m_audio_lib->stop();

        return;
    }

    m_audio_lib = new AudioStreamingLibCore(this);

    connect(m_audio_lib, &AudioStreamingLibCore::destroyed, [&]{m_audio_lib = nullptr;});

    QByteArray password = linewebpassword->text().toLatin1();

    QAudioDeviceInfo inputdevinfo = comboboxaudioinput->currentData().value<QAudioDeviceInfo>();
    QAudioDeviceInfo outputdevinfo = comboboxaudiooutput->currentData().value<QAudioDeviceInfo>();

    QAudioFormat format;
    format.setSampleSize(32);
    format.setSampleRate(linesamplerateweb->text().toInt());
    format.setChannelCount(linechannelsweb->text().toInt());
    format.setSampleType(QAudioFormat::Float);
    format.setByteOrder(QAudioFormat::LittleEndian);

    StreamingInfo info;

    info.setInputAudioFormat(format);
    info.setInputDeviceInfo(inputdevinfo);
    info.setOutputDeviceInfo(outputdevinfo);
    info.setWorkMode(StreamingInfo::StreamingWorkMode::WebClient);
    info.setTimeToBuffer(linebuffertime->text().toInt());
    info.setEncryptionEnabled(!password.isEmpty());
    info.setGetAudioEnabled(true);
    info.setNegotiationString(QByteArray("WalkieTalkieTCPDemo"));
    info.setID(linewebid->text().trimmed());

    buttonrecord->setEnabled(true);

    connect(m_audio_lib, &AudioStreamingLibCore::error, this, &MainWindow::error);

    connect(m_audio_lib, &AudioStreamingLibCore::inputLevel, levelinput, &LevelWidget::setlevel);
    connect(m_audio_lib, &AudioStreamingLibCore::outputLevel, leveloutput, &LevelWidget::setlevel);

    connect(m_audio_lib, &AudioStreamingLibCore::connectedToServer, this, &MainWindow::connectedToServer);
    connect(m_audio_lib, &AudioStreamingLibCore::connected, this, &MainWindow::webClientConnected);
    connect(m_audio_lib, &AudioStreamingLibCore::disconnected, this, &MainWindow::webClientDisconnected);

    connect(m_audio_lib, &AudioStreamingLibCore::pending, this, &MainWindow::pending);

    connect(m_audio_lib, &AudioStreamingLibCore::adjustSettings, this, &MainWindow::adjustSettings);

    connect(m_audio_lib, &AudioStreamingLibCore::extraData, this, &MainWindow::receiveText);

    connect(m_audio_lib, &AudioStreamingLibCore::finished, this, &MainWindow::finished);

    connect(boxinputmuted, &QCheckBox::toggled, m_audio_lib, &AudioStreamingLibCore::setInputMuted);

    m_audio_lib->start(info);

    m_audio_lib->setInputMuted(boxinputmuted->isChecked());

    m_audio_lib->setVolume(slidervolume->value());

    m_audio_lib->connectToHost(lineserverweb->text(), lineportserver->text().toInt(), password);

    webClientStarted(false);

    buttonstartweb->setText("Connecting...");

    buttonstartweb->setEnabled(false);

    tabwidget->setTabEnabled(1, false);
}

void MainWindow::client()
{
    if (m_audio_lib)
    {
        buttonrecord->setEnabled(false);

        m_audio_lib->stop();

        return;
    }

    m_audio_lib = new AudioStreamingLibCore(this);

    connect(m_audio_lib, &AudioStreamingLibCore::destroyed, [&]{m_audio_lib = nullptr;});

    QByteArray password = lineclientpassword->text().toLatin1();

    QAudioDeviceInfo inputdevinfo = comboboxaudioinput->currentData().value<QAudioDeviceInfo>();
    QAudioDeviceInfo outputdevinfo = comboboxaudiooutput->currentData().value<QAudioDeviceInfo>();

    StreamingInfo info;

    info.setInputDeviceInfo(inputdevinfo);
    info.setOutputDeviceInfo(outputdevinfo);
    info.setWorkMode(StreamingInfo::StreamingWorkMode::WalkieTalkieClient);
    info.setEncryptionEnabled(!password.isEmpty());
    info.setGetAudioEnabled(true);
    info.setNegotiationString(QByteArray("WalkieTalkieTCPDemo"));
    info.setID(lineclientid->text().trimmed());

    buttonrecord->setEnabled(true);

    connect(m_audio_lib, &AudioStreamingLibCore::error, this, &MainWindow::error);

    connect(m_audio_lib, &AudioStreamingLibCore::inputLevel, levelinput, &LevelWidget::setlevel);
    connect(m_audio_lib, &AudioStreamingLibCore::outputLevel, leveloutput, &LevelWidget::setlevel);

    connect(m_audio_lib, &AudioStreamingLibCore::connected, this, &MainWindow::clientConnected);
    connect(m_audio_lib, &AudioStreamingLibCore::disconnected, this, &MainWindow::clientDisconnected);

    connect(m_audio_lib, &AudioStreamingLibCore::adjustSettings, this, &MainWindow::adjustSettings);

    connect(m_audio_lib, &AudioStreamingLibCore::extraData, this, &MainWindow::receiveText);

    connect(m_audio_lib, &AudioStreamingLibCore::finished, this, &MainWindow::finished);

    connect(boxinputmuted, &QCheckBox::toggled, m_audio_lib, &AudioStreamingLibCore::setInputMuted);

    m_audio_lib->start(info);

    m_audio_lib->setInputMuted(boxinputmuted->isChecked());

    m_audio_lib->setVolume(slidervolume->value());

    m_audio_lib->connectToHost(linehost->text().trimmed(), lineport->text().toInt(), password);

    clientStarted(false);

    buttonconnect->setText("Connecting...");

    buttonconnect->setEnabled(false);

    tabwidget->setTabEnabled(1, false);
}

void MainWindow::server()
{
    if (m_audio_lib)
    {
        buttonrecord->setEnabled(false);

        m_audio_lib->stop();

        return;
    }

    m_audio_lib = new AudioStreamingLibCore(this);

    connect(m_audio_lib, &AudioStreamingLibCore::destroyed, [&]{m_audio_lib = nullptr;});

    QByteArray password = lineserverpassword->text().toLatin1();

    QAudioDeviceInfo inputdevinfo = comboboxaudioinput->currentData().value<QAudioDeviceInfo>();
    QAudioDeviceInfo outputdevinfo = comboboxaudiooutput->currentData().value<QAudioDeviceInfo>();

    QAudioFormat format;
    format.setSampleSize(32);
    format.setSampleRate(linesamplerate->text().toInt());
    format.setChannelCount(linechannels->text().toInt());
    format.setSampleType(QAudioFormat::Float);
    format.setByteOrder(QAudioFormat::LittleEndian);

    StreamingInfo info;

    info.setInputAudioFormat(format);
    info.setInputDeviceInfo(inputdevinfo);
    info.setOutputDeviceInfo(outputdevinfo);
    info.setWorkMode(StreamingInfo::StreamingWorkMode::WalkieTalkieServer);
    info.setTimeToBuffer(linebuffertime->text().toInt());
    info.setEncryptionEnabled(!password.isEmpty());
    info.setGetAudioEnabled(true);
    info.setNegotiationString(QByteArray("WalkieTalkieTCPDemo"));
    info.setID(lineserverid->text().trimmed());

    buttonrecord->setEnabled(true);

    connect(m_audio_lib, &AudioStreamingLibCore::error, this, &MainWindow::error);

    connect(m_audio_lib, &AudioStreamingLibCore::inputLevel, levelinput, &LevelWidget::setlevel);
    connect(m_audio_lib, &AudioStreamingLibCore::outputLevel, leveloutput, &LevelWidget::setlevel);

    connect(m_audio_lib, &AudioStreamingLibCore::connected, this, &MainWindow::serverConnected);
    connect(m_audio_lib, &AudioStreamingLibCore::disconnected, this, &MainWindow::serverDisconnected);

    connect(m_audio_lib, &AudioStreamingLibCore::pending, this, &MainWindow::pending);

    connect(m_audio_lib, &AudioStreamingLibCore::adjustSettings, this, &MainWindow::adjustSettings);

    connect(m_audio_lib, &AudioStreamingLibCore::extraData, this, &MainWindow::receiveText);

    connect(m_audio_lib, &AudioStreamingLibCore::finished, this, &MainWindow::finished);

    connect(boxinputmuted, &QCheckBox::toggled, m_audio_lib, &AudioStreamingLibCore::setInputMuted);

    m_audio_lib->start(info);

    m_audio_lib->setInputMuted(boxinputmuted->isChecked());

    m_audio_lib->setVolume(slidervolume->value());

    m_audio_lib->listen(lineportserver->text().toInt(), false, password);

    serverStarted(false);

    buttonstartserver->setText("Stop Server");

    tabwidget->setTabEnabled(1, false);
}

void MainWindow::sliderVolumeValueChanged(int value)
{
    if (m_audio_lib)
        m_audio_lib->setVolume(value);

    QString str = QString("Output Volume: %0").arg(value);

    labelvolume->setText(str);
}

void MainWindow::webClientStarted(bool enable)
{
    comboboxaudioinput->setEnabled(enable);
    comboboxaudiooutput->setEnabled(enable);
    widget2->setEnabled(enable);
    widget3->setEnabled(enable);

    lineserverweb->setEnabled(enable);
    lineportweb->setEnabled(enable);
    linewebid->setEnabled(enable);
    linewebpassword->setEnabled(enable);
    linesamplerateweb->setEnabled(enable);
    linechannelsweb->setEnabled(enable);
    linebuffertimeweb->setEnabled(enable);
}

void MainWindow::clientStarted(bool enable)
{
    comboboxaudioinput->setEnabled(enable);
    comboboxaudiooutput->setEnabled(enable);
    widget1->setEnabled(enable);
    widget3->setEnabled(enable);
    linehost->setEnabled(enable);
    lineport->setEnabled(enable);
    lineclientid->setEnabled(enable);
    lineclientpassword->setEnabled(enable);
}

void MainWindow::serverStarted(bool enable)
{
    comboboxaudioinput->setEnabled(enable);
    comboboxaudiooutput->setEnabled(enable);
    widget1->setEnabled(enable);
    widget2->setEnabled(enable);
    lineportserver->setEnabled(enable);
    linesamplerate->setEnabled(enable);
    linechannels->setEnabled(enable);
    linebuffertime->setEnabled(enable);
    lineserverid->setEnabled(enable);
    lineserverpassword->setEnabled(enable);
}

void MainWindow::connectedToServer(const QByteArray &hash)
{
    QString str = QString("Do you trust this certificate?\nFingerprint: %0").arg(QString(hash));

    int result = QMessageBox::question(this, "Accept?", str);

    if (!m_audio_lib)
        return;

    if (result != QMessageBox::Yes)
    {
        m_audio_lib->stop();

        return;
    }

    m_audio_lib->acceptSslCertificate();

    buttonconnecttopeer->setEnabled(true);

    buttonstartweb->setText("Disconnect from server");

    buttonstartweb->setEnabled(true);
}

void MainWindow::connectToPeer()
{
    QString id = QInputDialog::getText(this, "Set ID", "Select ID to connect:");

    if (!id.isEmpty())
        m_audio_lib->connectToPeer(id);
}

void MainWindow::webClientConnected(const QHostAddress &address, const QString &id)
{
    m_peer = QString("%0 - %1").arg(QHostAddress(address.toIPv4Address()).toString()).arg(id);

    QString title = QString("Connected to: %0 - %1").arg(m_peer).arg(TITLE);

    setWindowTitle(title);

    buttonconnecttopeer->setEnabled(false);

    buttonstartweb->setText("Disconnect from peer");
    buttonstartweb->setEnabled(true);

    linechat->blockSignals(false);
    buttonsendchat->setEnabled(true);

    boxautostart->setEnabled(false);

    if (boxautostart->isChecked())
        startPauseRecord(); //Auto start recording when connected
}

void MainWindow::clientConnected(const QHostAddress &address, const QString &id)
{
    if (!id.isEmpty())
        m_peer = QString("%0 - %1").arg(QHostAddress(address.toIPv4Address()).toString()).arg(id);
    else
        m_peer = QString("%0").arg(QHostAddress(address.toIPv4Address()).toString());

    QString title = QString("Connected to: %0 - %1").arg(m_peer).arg(TITLE);

    setWindowTitle(title);

    buttonconnect->setText("Disconnect");
    buttonconnect->setEnabled(true);

    linechat->blockSignals(false);
    buttonsendchat->setEnabled(true);

    boxautostart->setEnabled(false);

    if (boxautostart->isChecked())
        startPauseRecord(); //Auto start recording when connected
}

void MainWindow::serverConnected(const QHostAddress &address, const QString &id)
{
    if (!id.isEmpty())
        m_peer = QString("%0 - %1").arg(QHostAddress(address.toIPv4Address()).toString()).arg(id);
    else
        m_peer = QString("%0").arg(QHostAddress(address.toIPv4Address()).toString());

    QString title = QString("Connected to: %0 - %1").arg(m_peer).arg(TITLE);

    setWindowTitle(title);

    buttonstartserver->setText("Stop server");

    linechat->blockSignals(false);
    buttonsendchat->setEnabled(true);

    boxautostart->setEnabled(false);

    if (boxautostart->isChecked())
        startPauseRecord(); //Auto start recording when connected
}

void MainWindow::webClientDisconnected()
{
    if (m_audio_lib)
        m_audio_lib->stop();

    stopRecord();
    resetRecordPage();

    boxautostart->setEnabled(true);

    setWindowTitle(TITLE);

    buttonconnecttopeer->setEnabled(false);

    m_peer = QString();
}

void MainWindow::clientDisconnected()
{
    if (m_audio_lib)
        m_audio_lib->stop();

    stopRecord();
    resetRecordPage();

    boxautostart->setEnabled(true);

    setWindowTitle(TITLE);

    m_peer = QString();
}

void MainWindow::serverDisconnected()
{
    linechat->blockSignals(true);
    buttonsendchat->setEnabled(false);

    stopRecord();
    resetRecordPage();

    boxautostart->setEnabled(true);

    setWindowTitle(TITLE);

    m_peer = QString();
}

void MainWindow::pending(const QHostAddress &address, const QString &id)
{
    QString str = QString("Do you want to start a talk with %0?").arg(QHostAddress(address.toIPv4Address()).toString());

    if (!id.isEmpty())
        str.append(QString("\nID: %0").arg(id));

    int result = QMessageBox::question(this, "Pending connection", str);

    if (!m_audio_lib)
        return;

    if (result == QMessageBox::Yes)
        m_audio_lib->acceptConnection();
    else
        m_audio_lib->rejectConnection();
}

void MainWindow::error(const QString &error)
{
    if (!error.isEmpty())
        QMessageBox::critical(this, "Error", error);
}

void MainWindow::adjustSettings()
{
    if (!m_audio_lib)
        return;

    QAudioFormat inputFormat = m_audio_lib->inputAudioFormat();
    QAudioFormat format = m_audio_lib->audioFormat();

    QString str;

    str.append("Input format:\n\n");

    str.append(QString("Sample size: %0 bits\n").arg(inputFormat.sampleSize()));
    str.append(QString("Sample rate: %0 hz\n").arg(inputFormat.sampleRate()));
    str.append(QString("Channels: %0\n").arg(inputFormat.channelCount()));
    str.append(QString("Sample type: %0\n").arg((inputFormat.sampleType() == QAudioFormat::Float) ? "Float" : "Integer"));
    str.append(QString("Byte order: %0\n").arg((inputFormat.byteOrder() == QAudioFormat::LittleEndian) ? "Little endian" : "Big endian"));

    str.append("\n");

    str.append("Resampled format:\n\n");

    str.append(QString("Sample size: %0 bits\n").arg(format.sampleSize()));
    str.append(QString("Sample rate: %0 hz\n").arg(format.sampleRate()));
    str.append(QString("Channels: %0\n").arg(format.channelCount()));
    str.append(QString("Sample type: %0\n").arg((format.sampleType() == QAudioFormat::Float) ? "Float" : "Integer"));
    str.append(QString("Byte order: %0\n").arg((format.byteOrder() == QAudioFormat::LittleEndian) ? "Little endian" : "Big endian"));

    str.append(QString("\nBuffer time: %0 ms").arg(m_audio_lib->streamingInfo().timeToBuffer()));

    texteditsettings->setPlainText(str);
}

void MainWindow::setRecordPath()
{
    QString path = QFileDialog::getSaveFileName(this, "Select the wave path", QString(), "WAVE (*.wav)");

    if (path.isEmpty())
        return;

    linerecordpath->setText(QDir::toNativeSeparators(path));
}

void MainWindow::startPauseRecord()
{
    if (m_paused)
    {
        if (!m_audio_recorder)
        {
            linerecordpath->setEnabled(false);
            buttonsearch->setEnabled(false);
            buttonrecordstop->setEnabled(true);

            m_format = m_audio_lib->audioFormat();
            //Change to compatible settings
            m_format.setSampleSize(16);
            m_format.setSampleType(QAudioFormat::SignedInt);

            m_audio_recorder = new AudioRecorder(linerecordpath->text(), m_format, m_audio_lib);

            connect(m_audio_recorder, &AudioRecorder::destroyed, [&]{m_audio_recorder = nullptr;});

            if (!m_audio_recorder->open())
            {
                stopRecord();
                QMessageBox::critical(this, "Error", "Error openning file for record!");
                return;
            }
        }

        connect(m_audio_lib, &AudioStreamingLibCore::veryInputData, this, &MainWindow::writeLocalToBuffer);
        connect(m_audio_lib, &AudioStreamingLibCore::veryOutputData, this, &MainWindow::writePeerToBuffer);

        buttonrecord->setText("Pause");

        m_paused = false;
    }
    else
    {
        disconnect(m_audio_lib, &AudioStreamingLibCore::veryInputData, this, &MainWindow::writeLocalToBuffer);
        disconnect(m_audio_lib, &AudioStreamingLibCore::veryOutputData, this, &MainWindow::writePeerToBuffer);

        buttonrecord->setText("Record");

        m_paused = true;
    }
}

void MainWindow::stopRecord()
{
    if (m_audio_recorder)
    {
        disconnect(m_audio_lib, &AudioStreamingLibCore::veryInputData, this, &MainWindow::writeLocalToBuffer);
        disconnect(m_audio_lib, &AudioStreamingLibCore::veryOutputData, this, &MainWindow::writePeerToBuffer);

        m_audio_recorder->deleteLater();
    }

    linerecordpath->setEnabled(true);
    buttonsearch->setEnabled(true);
    buttonrecord->setText("Record");
    buttonrecordstop->setEnabled(false);

    m_paused = true;

    lcdtime->display("--:--");

    m_format = QAudioFormat();
}

void MainWindow::resetRecordPage()
{
    buttonsearch->setText("...");
    buttonrecord->setText("Record");
    buttonrecordstop->setText("Stop record");

    buttonrecord->setEnabled(false);
    buttonrecordstop->setEnabled(false);

    lcdtime->display("--:--");
}

void MainWindow::writeLocalToBuffer(const QByteArray &data)
{
    m_local_audio.append(data);

    mixLocalPeer();
}

void MainWindow::writePeerToBuffer(const QByteArray &data)
{
    m_peer_audio.append(data);

    mixLocalPeer();
}

void MainWindow::mixLocalPeer()
{
    int size = AudioStreamingLibCore::timeToSize(10, m_audio_lib->audioFormat());

    while (m_local_audio.size() >= size && m_peer_audio.size() >= size)
    {
        QByteArray mixed = AudioStreamingLibCore::mixFloatAudio(m_local_audio.mid(0, size), m_peer_audio.mid(0, size));

        m_local_audio.remove(0, size);

        m_peer_audio.remove(0, size);

        m_audio_recorder->write(AudioStreamingLibCore::convertFloatToInt16(mixed));

        qint64 recorded = AudioStreamingLibCore::sizeToTime(m_audio_recorder->size() - 44, m_format);

        QTime time = QTime(0, 0, 0).addMSecs(recorded);

        lcdtime->display(time.toString(Qt::ISODate));
    }
}

void MainWindow::writeText()
{
    if (!m_audio_lib->isReadyToWriteExtraData())
        return;

    QString text = linechat->text().trimmed();

    if (text.isEmpty())
        return;

    linechat->clear();

    QString localtext = QString("You wrote at %0:\n%1\n")
            .arg(QDateTime::currentDateTime().toString(Qt::SystemLocaleShortDate)).arg(text);

    texteditchat->appendPlainText(localtext);

    m_audio_lib->writeExtraData(text.toLatin1());
}

void MainWindow::receiveText(const QByteArray &data)
{
    QString text = QLatin1String(data);

    QString localtext = QString("%0 wrote at %1:\n%2\n").arg(m_peer)
            .arg(QDateTime::currentDateTime().toString(Qt::SystemLocaleShortDate)).arg(text);

    texteditchat->appendPlainText(localtext);

    m_audio_lib->writeExtraDataResult();
}

void MainWindow::finished()
{
    if (!isVisible())
        return;

    stopRecord();
    resetRecordPage();

    boxautostart->setEnabled(true);

    buttonconnecttopeer->setEnabled(false);

    webClientStarted(true);
    clientStarted(true);
    serverStarted(true);

    linechat->blockSignals(true);
    buttonsendchat->setEnabled(false);

    texteditsettings->clear();

    buttonconnect->setText("Connect");
    buttonconnect->setEnabled(true);

    buttonstartserver->setText("Start Server");
    buttonstartserver->setEnabled(true);

    buttonstartweb->setText("Connect to server");
    buttonstartweb->setEnabled(true);

    m_peer = QString();

    levelinput->setlevel(0);
    leveloutput->setlevel(0);

    tabwidget->setTabEnabled(1, true);

    if (m_audio_lib)
        m_audio_lib->deleteLater();

    m_local_audio.clear();
    m_peer_audio.clear();

    setWindowTitle(TITLE);
}

void MainWindow::getDevInfo()
{
    QList<QAudioDeviceInfo> inputdevices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    QList<QAudioDeviceInfo> outputdevices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);

    if (!inputdevices.isEmpty())
    {
        for (int i = 0; i < inputdevices.size(); i++)
            comboboxaudioinput->addItem(inputdevices.at(i).deviceName(), qVariantFromValue(inputdevices.at(i)));
    }
    else
    {
        QMessageBox::warning(this, "Error", "No input device found!");
    }

    if (!outputdevices.isEmpty())
    {
        for (int i = 0; i < outputdevices.size(); i++)
            comboboxaudiooutput->addItem(outputdevices.at(i).deviceName(), qVariantFromValue(outputdevices.at(i)));
    }
    else
    {
        QMessageBox::warning(this, "Error", "No output device found!");
    }
}
