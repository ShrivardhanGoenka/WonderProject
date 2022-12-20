#include "mainwindow.h"
#include "camera.h"
#include "ui_mainwindow.h"
#include "wondercam.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap pix(":/images/logo.png");
    int h = ui->logo->height();
    ui->logo->setPixmap(pix.scaled(h*3,h,Qt::KeepAspectRatio));

    maintimer = new QTimer(this);
    connect(maintimer,SIGNAL(timeout()),this,SLOT(timertimeout()));
    maintimer->start(20);

    ui->tabWidget->setTabsClosable(true);
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabSelected()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connect_clicked()
{
    ipDialog* obj = new ipDialog();
    if(obj->start()==0)
        return;


    int index;
    QWidget *tab;
    index = ui->tabWidget->insertTab(ui->tabWidget->count(), new QWidget(), QIcon(QString("")), "tab");
    tab = ui->tabWidget->widget(index);
    ui->tabWidget->setCurrentWidget(tab);

    if(obj->type==1){
        Camera* cam = new WonderCam(this,obj->devInfo,obj->pHandle);
        cam->inittab(tab);
        ui->tabWidget->setTabText(index,cam->camName);
        cameras.push_back(cam);
    }
    else if(obj->type == 2){
        Camera* cam = new Camera();
        cameras.push_back(cam);
    }


}

void MainWindow::timertimeout(){
    if(currenttab==-1) return;
    cameras[currenttab]->updateTab();
}

void MainWindow::closeTab(const int& indexofcam){
    if(indexofcam==-1) return;

    QWidget* widget = ui->tabWidget->widget(indexofcam);
    QString name = widget->objectName();

    //remove camera confirmation
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Disconnect Camera", QString("Are you sure you want to disconnect camera "+ name), QMessageBox::Yes|QMessageBox::No);

    if(reply==QMessageBox::Yes){
        Camera* cam = cameras[indexofcam];
        cam->closetab();
        cameras.erase(cameras.begin()+indexofcam);
        delete cam;
        ui->tabWidget->removeTab(indexofcam);
    }
}

void MainWindow::tabSelected(int index){
    currenttab = index;
}
