#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QDir>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QFileInfo>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);
    videoWidget = new QVideoWidget(this);
    videoWidget->resize(600,300);
    player->setVideoOutput(videoWidget);
    //player->setMedia(QUrl::fromLocalFile("C:/Users/ASUS/Desktop/11.mp4"));

    connect(player,&QMediaPlayer::positionChanged,this,&Widget::updatePosition);


    //player->play();
    //亮度
    ui->horizontalSlider_2->setValue(videoWidget->brightness());
    connect(ui->horizontalSlider_2,&QSlider::sliderMoved,videoWidget,&QVideoWidget::setBrightness);
    connect(videoWidget,&QVideoWidget::brightnessChanged,ui->horizontalSlider_2,&QSlider::value);
    //对比度
    ui->horizontalSlider_3->setValue(videoWidget->contrast());
    connect(ui->horizontalSlider_3,&QSlider::sliderMoved,videoWidget,&QVideoWidget::setContrast);
    connect(videoWidget,&QVideoWidget::contrastChanged,ui->horizontalSlider_3,&QSlider::value);
    //色相
    ui->horizontalSlider_4->setValue(videoWidget->hue());
    connect(ui->horizontalSlider_4,&QSlider::sliderMoved,videoWidget,&QVideoWidget::setHue);
    connect(videoWidget,&QVideoWidget::hueChanged,ui->horizontalSlider_4,&QSlider::value);
    //饱和度
    ui->horizontalSlider_5->setValue(videoWidget->saturation());
    connect(ui->horizontalSlider_5,&QSlider::sliderMoved,videoWidget,&QVideoWidget::setSaturation);
    connect(videoWidget,&QVideoWidget::saturationChanged,ui->horizontalSlider_5,&QSlider::value);

    model = new QStandardItemModel(this);
    strListFileName.clear();//文件名称对应的列表
    strListUrlName.clear();
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::updatePosition(qint64 position){
    //更新滑块显示并更新时间
    ui->horizontalSlider->setMaximum(player->duration()/1000);
    ui->label_3->setText(ChangeTime(player->duration()/1000));
    ui->label->setText(ChangeTime(position/1000));
    ui->horizontalSlider->setValue(position/1000);
}

QString Widget::ChangeTime(qint64 position){
    int min = position/60;
    int second = position%60;
    int hour = 0;
    if(min>-60){
        hour = min/60;
        min = min%60;
    }
    QString strhour = (hour>9)?QString::number(hour):tr("0")+QString::number(hour);
    QString strmin = (min>9)?QString::number(min):tr("0")+QString::number(min);
    QString strsecond = (second>9)?QString::number(second):tr("0")+QString::number(second);
    return strhour+":"+strmin+":"+strsecond;
}

void Widget::on_pushButton_clicked()
{
   player->play();
}

void Widget::on_pushButton_2_clicked()
{
    player->pause();
}

void Widget::on_pushButton_3_clicked()
{
    player->stop();
}

void Widget::on_pushButton_4_clicked()
{
    //qDebug()<<"点击打开文件";
    QString filename = QFileDialog::getOpenFileName(this,"open Video",QDir::currentPath(),"video file(*.mp4);;ALL file(*.*)");
    player->setMedia(QUrl::fromLocalFile(filename));
    player->play();
}

void Widget::on_pushButton_5_clicked()
{
    qDebug()<<"点击打开目录";
    strListFileName.clear();//文件名称对应的列表
    strListUrlName.clear();
    model->clear();
    player->stop();

    QString dirname = QFileDialog::getExistingDirectory(this,tr("open Directiry"),".");
    QDir dir(dirname);
    dir.setNameFilters(QStringList("*.mp4"));
    strListFileName = dir.entryList();//获取文件列表
    //qDebug()<<strListFileName;
    QString strPathName = dir.absolutePath();//获取目录路径
    qDebug()<<"strPathName:"+strPathName;
    int count = strListFileName.count();

    for(int i=0;i<count;i++){
        QStandardItem *item = new QStandardItem(strListFileName.at(i));
        model->appendRow(item);
        strListUrlName.append(strPathName+"/"+strListFileName.at(i));
        qDebug()<<"strListUrlName:";
        qDebug()<<strListUrlName;
    }
    qDebug()<<"******************";
    ui->listView->setModel(model);
}

void Widget::on_listView_doubleClicked(const QModelIndex &index)
{
    //qDebug()<<"双击列表";
    player->setMedia(QUrl::fromLocalFile(strListUrlName.at(index.row())));
    qDebug()<<"strListUrlName:"+strListUrlName.at(index.row());
    player->play();
}

void Widget::on_horizontalSlider_sliderMoved(int position)
{
    //设置播放进度
    player->setPosition(position);
}
