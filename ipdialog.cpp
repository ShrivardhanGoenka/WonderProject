#include "ipdialog.h"
#include "ui_ipdialog.h"

ipDialog::ipDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ipDialog)
{
    ui->setupUi(this);
    ui->usernameEdit->insert("admin");
    ui->passwordEdit->insert("123456");
    ui->ipEdit->addItem("192.168.1.108");

    //Adding all the camera types
    for(auto i:cameraType)
        ui->cameraMenu->addItem(i);

    comboChange(0);

    connect(ui->cameraMenu, SIGNAL(activated(int)), this, SLOT(comboChange(int)));
}

ipDialog::~ipDialog()
{
    delete ui;
}

void ipDialog::loadsearch(){
    movie= new QMovie(":/images/loader1.gif");
    ui->load->setMovie(movie);
    ui->load->show();
    movie->start();
}

//Main function to load the dialog and check for validity
int ipDialog::start(){

    int flag = 0;
    while(!flag){
        //launching the dialog
        int temp = this->exec();
        if(temp==0) return 0;
        //Camera Type 1
        if(information.type==1){
            pHandle = sdk_initialize(1,devInfo.szUserName, devInfo.szPWD, devInfo.szIP);
            if (pHandle <= (IRNETHANDLE)0)
            {
                QMessageBox::information(this, "Error", QString("sdk_initialize failed. Please try again"));
                continue;
            }
            int ret = sdk_login(pHandle, devInfo);
            if(ret!=0){
                QMessageBox::critical(this, "Login failed", QString("Unable to login! \nPlease check login credentials.\nPlease try again."));
                continue;
            }
            flag =1;
        }
        //Camera Type 2
        else if(information.type == 2){
            if(information.username=="Username" && information.password=="Password"){
                flag = 1;
            }
            else{
                QMessageBox::critical(this, "Login failed", QString("Unable to login! \nPlease check login credentials.\nPlease try again."));
                continue;
            }
        }
    }
    return 1;
}

//start search procedure for all cameras

//camera 1
//Wonder Cam
void ipDialog::onStarted_cam1(){
    res = QtConcurrent::run([this] ()->int {return sdk_search_device(1, deviceList);});
    connect(&futWatch, &QFutureWatcher<int>::finished, this, &ipDialog::onFinished_cam1);
    futWatch.setFuture(res);
}

//finished searching functions for all cameras

//camera 1
//Wonder Cam
void ipDialog::onFinished_cam1(){
    movie->stop();
    ui->load->clear();
    int result =  futWatch.result();
    qDebug() << "sdk_serach returned:" << result;
    if(result==-1)
    {
        QMessageBox::information(this, "Error", QString("Search failed"));
        return;
    }
    for(int i=0; i<deviceList.number; i++){
        strcpy_s(ip, deviceList.device_param[i].devInfo[0].szIP);
        ui->ipEdit->addItem(ip);
    }
    ui->ipEdit->setCurrentIndex(0);
}

//function for when search button is clicked in the Dialog Box
void ipDialog::on_search_clicked()
{
    loadsearch();
    if(type==1){
        onStarted_cam1();
    }
}

ipDialog::info ipDialog::get_information(){
    return information;
}

void ipDialog::on_login_clicked(){
    information.ip = ui->ipEdit->currentText();
    information.password = ui->passwordEdit->text();
    information.username = ui->usernameEdit->text();
    information.type = this->type;
    //Writing the devInfo object when login is clicked
    if(this->type == 1){
        strcpy_s(devInfo.szUserName, ui->usernameEdit->text().toStdString().c_str());
        strcpy_s(devInfo.szPWD, ui->passwordEdit->text().toStdString().c_str());
        strcpy_s(devInfo.szIP, ui->ipEdit->currentText().toStdString().c_str());
        strcpy_s(devInfo.mac, "");
        strcpy_s(devInfo.netDev, "");
        devInfo.channel = 0;
        devInfo.isDefault = 0;
    }
}

//function to monitor the change in the Camera Type and alter ui based on it
void ipDialog::comboChange(int arg){
    if(ui->cameraMenu->itemText(arg) == "Select Camera Type"){
        //Default configuration. Set all but cancel to disabled
        ui->ipEdit->setDisabled(true);
        ui->ipLabel->setDisabled(true);
        ui->passwordEdit->setDisabled(true);
        ui->passwordLabel->setDisabled(true);
        ui->usernameEdit->setDisabled(true);
        ui->usernameLabel->setDisabled(true);
        ui->login->setDisabled(true);
        ui->search->setDisabled(true);
        //Set type to 0
        type = 0;
    }
    if(ui->cameraMenu->itemText(arg)==cameraType[0]){ // "Wonder Cam"
        //Camera Type 1
        ui->ipEdit->setDisabled(false);
        ui->ipLabel->setDisabled(false);
        ui->passwordEdit->setDisabled(false);
        ui->passwordLabel->setDisabled(false);
        ui->usernameEdit->setDisabled(false);
        ui->usernameLabel->setDisabled(false);
        ui->login->setDisabled(false);
        ui->search->setDisabled(false);
        //Set type to 1
        type = 1;
    }
    else if(ui->cameraMenu->itemText(arg)==cameraType[1]){ // "Camera Type 2"
        //change configuration depending on input requirements of this camera.
        ui->ipEdit->setDisabled(true);
        ui->ipLabel->setDisabled(true);
        ui->passwordEdit->setDisabled(false);
        ui->passwordLabel->setDisabled(false);
        ui->usernameEdit->setDisabled(false);
        ui->usernameLabel->setDisabled(false);
        ui->login->setDisabled(false);
        ui->search->setDisabled(true);
        //Set type to 2
        type = 2;
    }
    else if(ui->cameraMenu->itemText(arg)==cameraType[2]){ //"Camera Type 3"
        //change configuration depending on input requirements of this camera.
        ui->ipEdit->setDisabled(false);
        ui->ipLabel->setDisabled(false);
        ui->passwordEdit->setDisabled(true);
        ui->passwordLabel->setDisabled(true);
        ui->usernameEdit->setDisabled(false);
        ui->usernameLabel->setDisabled(false);
        ui->login->setDisabled(false);
        ui->search->setDisabled(true);
        //Set type to 3
        type = 3;
    }
}



