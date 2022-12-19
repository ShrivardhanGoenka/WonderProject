#ifndef WONDERCAM_H
#define WONDERCAM_H

#include "IRSThermalCameraSDK.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "camera.h"
#include "mainwindow.h"
#include "qgridlayout.h"
#include "qlabel.h"
#include <QRadioButton>
#include <QMessageBox>
#include <QProcess>
#include <QDir>

using namespace cv;

class WonderCam: public Camera
{
public:
    WonderCam(){};
    WonderCam(QObject*,DeviceInfo, IRNETHANDLE);
    void inittab(QWidget*) override;
    void closetab() override;
    void updateTab() override;

    static void PtzAlarmCallBack(char* message,void* context);
    Q_SLOT void recordAlarms(int type, int action, QString name);
    Q_SLOT void captureAlarms(int type,int action, QString name);
    Q_SIGNAL void recordSignal(int, int, QString);
    Q_SIGNAL void captureSignal(int, int, QString);

    static WonderCam& instance() {
        static WonderCam m_instance;
        return m_instance;
    }

private:
    static int index;
    void onRadioButtonToggled();

    void update_window();
    void update_cctv();

    //SDK variables
    PTZ_SystemInfo systemInfo;
    DeviceInfo devInfo;
    IRNETHANDLE pHandle;

    //Variables for real-time video running
    cv::Mat frame;
    cv::VideoCapture capture;
    cv::Mat cappedFrame;
    char rtsp[256];
    QTimer* timer;

    //Variables for CCTV running
    cv::VideoCapture captureC;
    cv::Mat frameC;
    char rtspC[256];
    QTimer* timerC;

    //recording
    QProcess* p;

    QString partpath;
    QStringList pArguments;
    QStringList pArguments2;

    QProcess* proc[5];
    QString procstr[5];

    //alarm path
    QString alarmpath;

    //stream label
    QLabel* streamlabel;
    QLabel* titleLabel;

    bool capORrec = 1;

    //GUI Objects
    QRadioButton* qrbRec;
    QRadioButton *qrbCap;
    QVBoxLayout *radioLayout;
    QGridLayout *tabgridlayout;
    QLabel *liveLabelC;
    QLabel *liveLabel;
};

#endif // WONDERCAM_H
