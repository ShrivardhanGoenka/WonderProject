#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <ipdialog.h>
#include <QTimer>
#include <QMainWindow>
#include <QPixmap>
#include <QImage>
#include <QRadioButton>
#include <camera.h>
#include <QTimer>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <thread>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static MainWindow& instance() {
        static MainWindow m_instance;
        return m_instance;
    }

private slots:
    void on_connect_clicked();
    void timertimeout();
    void closeTab(const int& index);

private:
    Ui::MainWindow *ui;
    vector<Camera*> cameras;
    QTimer* maintimer;
};

#endif // MAINWINDOW_H
