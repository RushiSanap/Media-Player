#include "mainwindow.h"
#include "ui_mainwindow.h"


float rate = 1;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    init();//initializes the ui

    AdvDialog = new class AdvDialog(this,vw);

    list <QString> names;//list to maintain playlist

    signalmapper = new QSignalMapper(this);//mapping playlist signal to slots

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::init()
{
    ui->setupUi(this);
    vw = new QVideoWidget(this);    //provides a widget which presents video produced by a media object

    player = new QMediaPlayer(this);    //allows the playing of a media source

    volume = new QSlider(this);     //provide the volume slider

    currentContentSlider = new QSlider(this);   //slider for moving as per video content

    currentContentDuration = new QLabel("00:00/00:00");     //initially current content durtion


    player->setVideoOutput(vw);

    this->setWindowTitle("MediaPlayer");    //setting the window title

    volume->setOrientation(Qt::Horizontal);     //orientation of the volume slider
    volume->setValue(100);      //volume level upto 100
    volume->setMaximumWidth(200);       //width of the volume slider

    ui->toolBar->addWidget(volume);     //placing the volume slider into the toolbar

    currentContentSlider->setOrientation(Qt::Horizontal);       //set orientation of curent content slider horizontally
    currentContentSlider->setFixedWidth(1000);      //width of curent content slider
    currentContentSlider->setFixedHeight(35); //heigth of curent content slider    
    currentContentSlider->setStyleSheet("background-color: #E5E5E2");


    ui->statusbar->addPermanentWidget(currentContentSlider);    //placing the current content slider into the status bar
    ui->statusbar->addPermanentWidget(currentContentDuration);  //placing the current content duration label into the status bar ahead of current content slider


    ui->centralwidget->setStyleSheet("image: url(:/images/icons/4308765.jpg);");

    Playlist = menuBar()->addMenu(tr("&Playlist"));//placing custom menu on the menu bar


    initSignalsAndSlots();
 }


void MainWindow::initSignalsAndSlots()
{
    //connect the media player with the current content slider

    connect(player, &QMediaPlayer::durationChanged, currentContentSlider, &QSlider::setMaximum);

    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);

    connect(currentContentSlider, &QSlider::sliderMoved, player, &QMediaPlayer::setPosition);

    connect(volume,&QSlider::sliderMoved, player,&QMediaPlayer::setVolume);

}


void MainWindow::positionChanged(qint64 position)
{
    currentContentSlider->setValue(position);
    updateDurationInfo();//at every instant it will update the current content duration lable
}


void MainWindow::updateDurationInfo()
{
    //it will give the duration time at a particular instant where the tip of the slider is pointing on the
    //current content slider and store it into currentinfo variable
    const qint64 currentInfo = player->position() / 1000;

    //this will give the total duration time of the video and store it into duraation variable
    const qint64 duration = player->duration() / 1000;
    QString tStr;

    if (currentInfo)
    {
        //calcuation is needed for converting the current time into the string format mm:ss
        QTime currentTime((currentInfo / 3600) %60,
                          (currentInfo / 60) % 60,
                          (currentInfo % 60),
                          (currentInfo * 1000) % 1000);
        QTime totalTime((duration / 3600) % 60,
                        (duration / 60) % 60,
                        (duration % 60),
                        (duration * 1000) % 1000);
        QString format = "mm:ss";//if the toal duration time of video is less than 1 hour then the string format will be like this

        if (duration > 3600)
            format = "hh:mm:ss";//if the toal duration time of video is grater than 1 hour then the string format will be like this

        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);//covert the number getting through calculation
                                                                                 //with the mentioned string format
    }

    currentContentDuration->setText(tStr);
}


void MainWindow::playnew(QString s){
    //play the newly selected file from the playlist

    list <QString> :: iterator it;

    for(it = names.begin(); it != names.end(); ++it){
        if(*it == s){
            player->stop();

            player->setMedia(QUrl::fromLocalFile(s));
            player->play();

            ui->statusbar->showMessage("Playing");
            return;
        }
    }
    names.push_front(s);
    player->stop();

    player->setMedia(QUrl::fromLocalFile(s));
    player->play();
    ui->statusbar->showMessage("Playing");

}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,"Open a File","C://","Video File (*.mp4, *.mpg, *.avi, *.MP4)");
    if(filename == nullptr)
        return;

    on_actionStop_triggered();

    list <QString> :: iterator it;
    int found = 0;
    for(it = names.begin(); it != names.end(); ++it){
        if(*it == filename)
            found = 1;
    }

    if(!found){
        names.push_front(filename);

        QAction *act = new QAction(filename,this);//creating new action in playlist

        connect(act, SIGNAL(triggered()), signalmapper, SLOT(map()));//connecting trigger of action to custom slot
        signalmapper->setMapping(act, filename);
        connect(signalmapper,SIGNAL(mapped(QString)),this,SLOT(playnew(QString)));

        Playlist->addAction(act);
    }

    player->stop();
    this->setCentralWidget(vw);
    player->setMedia(QUrl::fromLocalFile(filename));

    on_actionPlay_triggered();//for starting the video as soon as the file is opened
}


void MainWindow::on_actionStop_triggered()
{
    player->stop();
    ui->statusbar->showMessage("Stopped");
}


void MainWindow::on_actionPlay_triggered()
{
    static int press = 0;
    if(press%2 == 1){
        player->pause();
        ui->statusbar->showMessage("Paused");
    }
    else{
        player->play();
        ui->statusbar->showMessage("Playing");
    }
    press++;

}

void MainWindow::on_actionAdv_triggered()
{
    AdvDialog->show();//show the window of the advance tools having the sliders for controlling the hue,brigthness, contrast and saturation
}

void MainWindow::on_actionForward_triggered()
{
    if(rate < 2){
        rate = rate + 0.2;
        player->setPlaybackRate(rate);//on every triggering playback speed will increase by 0.2x as long as it is less than twice the normal speed
    }
}

void MainWindow::on_actionSlow_down_triggered()
{
    if(rate > 0.6){
        rate = rate - 0.2;
        player->setPlaybackRate(rate);//on every triggering playback speed will decrease by 0.2x as long as it is greater than 0.4x the normal speed
    }
}

void MainWindow::on_actionNext_triggered()
{
    //stores the newly played file at the brginning of the list

    list <QString> :: iterator it;
    if(names.size() == 0)
        return;
    it = names.end();
    it--;
    QString newfile = *it;

    player->stop();
    this->setCentralWidget(vw);
    player->setMedia(QUrl::fromLocalFile(newfile));
    names.push_front(newfile);
    names.pop_back();

    on_actionPlay_triggered();
}

void MainWindow::on_actionPrevious_triggered()
{
    //it will play the previous file, remove it from end of list and append it to the front
    list <QString> :: iterator it;
    if(names.size() == 0)
        return;
    it = names.begin();
    QString currentfile = *it;
    ++it;
    QString prevfile = *it;

    player->stop();
    this->setCentralWidget(vw);
    player->setMedia(QUrl::fromLocalFile(prevfile));
    names.pop_front();
    names.pop_front();
    names.push_front(currentfile);
    names.push_front(prevfile);

    on_actionPlay_triggered();
}
