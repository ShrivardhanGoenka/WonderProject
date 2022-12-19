#ifndef IPDIALOG_H
#define IPDIALOG_H

#include <vector>
#include <QDialog>
#include <QMovie>
#include "IRSThermalCameraSDK.h"
#include <QFuture>
#include <QtConcurrent/qtconcurrentrun.h>
#include <QFutureWatcher>
#include <QMessageBox>
#include <string>
#include <thread>

using namespace std;

namespace Ui {
class ipDialog;
}

class ipDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ipDialog(QWidget *parent = nullptr);
    ~ipDialog();

    std::vector<QString> cameraType = {"Wonder Cam",
                                      "WebCam",
                                      "Camera Type 3"};

    struct info{
        QString ip;
        QString password;
        QString username;
        int type;
    };

    //Objects for Wonder Camera
    DeviceInfo devInfo;
    IRNETHANDLE pHandle;

    int start();

    int type = -1;//stores the type of camera selected. Default is -1 and will be altered in comboChange

 private slots:
    void on_search_clicked();
    void loadsearch();
    void onFinished_cam1();
    void onStarted_cam1();
    void comboChange(int);
    void on_login_clicked();
    info get_information();

private:
    info information;
    Ui::ipDialog *ui;

    //All user entered information after pressing log in

    char ip[30];
    QMovie* movie;

    QFuture<int> res;
    QFutureWatcher<int> futWatch;
    int result = 0;
    DeviceList deviceList;
};

#endif // IPDIALOG_H
