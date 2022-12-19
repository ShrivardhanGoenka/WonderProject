#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QWidget>
#include <QImage>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent = nullptr);

    virtual void closetab(){};
    virtual void inittab(QWidget* tab){}
    virtual void updateTab(){}

    QImage MatToQImage(const cv::Mat& mat);
    QString camName;
    QWidget* tab;
    QObject* parent;
signals:

};

#endif // CAMERA_H
