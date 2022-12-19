#include "wondercam.h"

WonderCam::WonderCam(QObject* p,DeviceInfo devInfo,IRNETHANDLE pHandle)
{
    Camera::parent = p;
    this->devInfo = devInfo;
    this->pHandle = pHandle;
}

void WonderCam::inittab(QWidget* tab){
    sdk_ptz_get_systeminfo(pHandle,devInfo,&systemInfo);

    char c = 'a';
    camName = strncat(systemInfo.serialNumber,&c,1);

    tab->setObjectName(camName);

    string _userName = devInfo.szUserName;
    string str = "rtsp://" + _userName + ":" + devInfo.szPWD + "@" + devInfo.szIP + "/cam/realmonitor?channel=2&subtype=1";
    memcpy(rtsp, str.c_str(), 256);
    str = "rtsp://" + _userName + ":" + devInfo.szPWD + "@" + devInfo.szIP + "/cam/realmonitor?channel=1&subtype=1";
    memcpy(rtspC, str.c_str(), 256);

    for (int j = 0; j < 5; j++) {
        proc[j] = nullptr;
        procstr[j] = "";
        //proct[j] = nullptr;
    }

    Camera::tab = tab;
    liveLabel = new QLabel();
    liveLabel->setObjectName(QString(camName+"livestream"));
    liveLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    liveLabel->setMinimumSize(0,0);
    liveLabel->setMaximumSize(700,600);
    liveLabel->setStyleSheet(        "border:1px solid black;"
                                     "border-radius: 5px;");
    liveLabelC = new QLabel();
    liveLabelC->setObjectName(QString(camName+"livestreamc"));
    liveLabelC->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    liveLabelC->setMinimumSize(0,0);
    liveLabelC->setMaximumSize(700,600);
    liveLabelC->setStyleSheet(        "border:1px solid black;"
                                     "border-radius: 5px;");
    tabgridlayout = new QGridLayout;
    tabgridlayout->addWidget(liveLabel);
    tabgridlayout->addWidget(liveLabelC, 0, 1);
    tab->setLayout(tabgridlayout);
    qrbRec = new QRadioButton("Record");
    qrbRec->setObjectName(QString("recRadio"));
    qrbRec->setChecked(true);
    qrbCap = new QRadioButton("Capture");
    qrbCap->setObjectName(QString("capRadio"));
    radioLayout = new QVBoxLayout;
    radioLayout->addWidget(qrbRec);
    radioLayout->addWidget(qrbCap);
    qrbRec->setStyleSheet(QString(
                              "QRadioButton"
                              "{"
                              "    background: transparent;"
                              "               color: black;"
                              "}"
                              ).arg(25));
    qrbCap->setStyleSheet(QString(
                              "QRadioButton"
                              "{"
                              "    background: transparent;"
                              "               color: black;"
                              "}"
                              ).arg(25));
    tabgridlayout->addLayout(radioLayout, 1,0);

    QObject::connect(qrbRec, &QAbstractButton::toggled, Camera::parent, [this]{onRadioButtonToggled();});
    QObject::connect(qrbCap, &QAbstractButton::toggled, Camera::parent, [this]{onRadioButtonToggled();});

    // record/capture store path
    QDir dir;

    dir.mkpath("C:/wonderApp");
    alarmpath = "C:/wonderApp/cam" + camName;

    dir.mkpath("C:/wonderApp/cam" + camName);
    /*cam->partpath = QString("C:/wonderApp/cam" + QString::fromStdString(devInfo.szUserName));*/
    pArguments << "-rtsp_transport" << "tcp" << "-i" << QString::fromStdString(rtsp) << "-vcodec" << "copy" << "-t" << "10" << "-y" << alarmpath; //Building list of paramiters to ffmpeg
    pArguments2 << "-i"<< rtsp<< "-f" <<"image2" << "-t" << "10" << "-r" <<"1/8"<<alarmpath;

    capture.open(rtsp);
    captureC.open(rtspC);

    SetAlarmCallBack(pHandle,devInfo,PtzAlarmCallBack,parent);
}

void WonderCam::onRadioButtonToggled(){
    QRadioButton* rb = qobject_cast<QRadioButton*>(sender());
    if(rb->objectName()=="recRadio"){
        if(rb->isChecked()){
            qDebug()<<"record is checked";
            capORrec=1;
        }
        else{
            qDebug()<<"capture is checked";
            capORrec=0;
        }
    }
    else if(rb->objectName()=="capRadio"){
        if(rb->isChecked()){
            qDebug()<<"capture is checked";
            capORrec=0;
        }
        else{
            qDebug()<<"record is checked";
            capORrec=1;
        }
    }
}

//safely close tab
void WonderCam::closetab(){
    capture.release();
    captureC.release();
}

void WonderCam::updateTab(){
    update_cctv();
    update_window();
}

void WonderCam::update_cctv(){
    captureC >> frameC;
    //cvtColor(cam->frameC, cam->frameC, cv::COLOR_BGR2BGRA);
    QImage imagec = MatToQImage(frameC);
    liveLabelC->setPixmap(QPixmap::fromImage(imagec).scaled(liveLabelC->width(), liveLabelC->height(), Qt::KeepAspectRatio));
}

void WonderCam::update_window(){
    capture >> frame;
    cvtColor(frame, frame, cv::COLOR_BGR2BGRA);
    QImage image = MatToQImage(frame);
    liveLabel->setPixmap(QPixmap::fromImage(image).scaled(liveLabel->width(), liveLabel->height(), Qt::KeepAspectRatio));
}

void WonderCam::PtzAlarmCallBack(char* message, void* context)
{
    AlarmData* alarm = (AlarmData*)message;

    /*if (alarm->action == 0) {
        emit instance().recordSignal(alarm->code, alarm->action, QString::fromStdString(alarm->heatImagingTemper_Stop.name));
        emit instance().captureSignal(alarm->code, alarm->action, QString::fromStdString(alarm->heatImagingTemper_Stop.name));
    }*/
    if (alarm->action == 1) {
        emit instance().recordSignal(alarm->code, alarm->action, QString::fromStdString(alarm->heatImagingTemper_Start.name));
        emit instance().captureSignal(alarm->code, alarm->action, QString::fromStdString(alarm->heatImagingTemper_Start.name));
    }

}
void WonderCam::recordAlarms(int type, int action, QString name)
{
    int n = 0;
    if (capORrec == 1) {
        if (action == 0) {
        }
        else if (action == 1) {
            // start record
            qInfo() << "start 0" << name;
            int i = 0;
            while (proc[i] != nullptr) {
                i++;
            }
            if (i >= 5) {
                i = 0;
            }
            proc[i] = new QProcess(this);
            procstr[i] = name;
            auto stamp = QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss");      //Get current timestamp

            pArguments.removeLast();
            pArguments.append(QString(alarmpath + "/" + camName + "-" + stamp + name + ".mp4"));
            proc[i]->start("ffmpeg", pArguments);
        }
    }

}
void WonderCam::captureAlarms(int type, int action, QString name)
{
    int n = 0;
    if (capORrec == 0) {
        if (action == 0) {
            qInfo() << "stop 0" << name;
        }
        else if (action == 1) {
            // start capture
            qInfo() << "start 0" << name;
            int i = 0;
            while (proc[i] != nullptr) {
                i++;
            }
            if (i >= 5) {
                i = 0;
            }
            proc[i] = new QProcess(this);
            procstr[i] = name;
            auto stamp = QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss");      //Get current timestamp

            pArguments2.removeLast();
            pArguments2.append(QString(alarmpath + "/" + camName + "-" + stamp + name + "-figure%01d.jpg"));
            proc[i]->start("ffmpeg", pArguments2);
        }

    }
}
