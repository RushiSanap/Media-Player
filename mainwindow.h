#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QProgressBar>
#include <QDialog>
#include <QSlider>
#include<QTime>
#include<QString>
#include<QLabel>
#include <QFileDialog>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include "advdialog.h"
#include <list>
#include <iterator>
#include <QSignalMapper>
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
    void init();

private slots:
    void on_actionOpen_triggered();//function for taking the video input from the system folder

    void on_actionPlay_triggered();//play or pause the video

    void on_actionStop_triggered();//stop the video and start it from the beginning

    void positionChanged(qint64 position);

    void on_actionAdv_triggered();//for opening the new window in which brighness, hue, saturation and contrast control sliders are their

    void on_actionForward_triggered();//to increase the speed of the video

    void on_actionSlow_down_triggered();//to slow down speed of theh video

    void on_actionNext_triggered();//to play next file in the playlist

    void on_actionPrevious_triggered();//to play previous file in the playlist

    void playnew(QString s);//play new file from playlist

private:
    Ui::MainWindow *ui;
    QMediaPlayer* player;
    QVideoWidget* vw;
    QSlider* volume;
    QSlider* currentContentSlider;
    QLabel* currentContentDuration;

    AdvDialog *AdvDialog;

    QMenu *Playlist;
    list <QString> names;
    QSignalMapper *signalmapper;

    void initSignalsAndSlots();
    void updateDurationInfo();

};
#endif // MAINWINDOW_H
