#include "PlayerView.h"
#include <QtAV>
#include <QtAVWidgets>
#include <QGraphicsProxyWidget>
#include <QGraphicsTextItem>
#include <QLabel>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QLineEdit>
#include <QOpenGLWidget>
#include <QListWidget>
#include <QPushButton>
#include "niceslider.h"
#include "NiceTimeSlider.h"
#include "UpWardComboBox.h"
#include "nicexml.h"
#include "niceDragItemRenderer.h"
#include "sliceframe.h"
#include "CustomPushButton.h"
#include "BorderFrame.h"

PlayerView::PlayerView(QWidget *parent):
    QGraphicsView(parent)
  , player(0)
  , playlistFrame(0)
  , hltCtl(0)
  , timeSlider(0)
  , videoItem(0)
  , scene(0)
  , frame(0)
  , sp1(0)
  , sp2(0)
  , l(0)
  , optL(0)
  , toolTipItem(0)
  , playButton(0)
  , volumeButton(0)
  , volumeSlider(0)
  , currentPosLabel(0)
  , rateBox(0)
  , maxButton(0)
  , editButton(0)
  , shareButton(0)
  , closeButton(0)
  , proxyFrame(0)
  , proxyCloseButton(0)
  , m_preview(0)
  , path("NULL")
  , xmlSettings(0)
  , m_unit(1000)
  , m_previewActivated(true)
  , m_previewSize(QSize(100,80))
  , m_previewShowTime(5000)
  , m_initalVolume(50)
  , m_currPosViewActivated(false)
  , m_tickWidthRatio(0.06)
  , m_tickHeightRatio(0.6)
  , m_timeSliderPos(CENTER_POS)
  , m_frameHeightRatio(0.15)
  , m_defaultPlayBackRate(true)
  , m_enableHideControlFrame(true)
  , startTime(-1)
  , endTime(-1)
  , startTimeROI(-1)
  , endTimeROI(-1)
  , repeat(false)
  , closeFlag(false)
  , closeButtonEnabled(true)
  , startTrackPos(-1)
  , endTrackPos(-1)
  , useOpenGL(false)
{
    QtAV::Widgets::registerRenderers();
    QtAV::setLogLevel(QtAV::LogCritical);
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    this->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));

}

PlayerView::~PlayerView()
{
    if(scene != NULL){
        this->deleteProxyWidgets();
        scene->clear();
        this->setScene(0);
        scene->deleteLater();
    }

    if(hltCtl != NULL){
        delete hltCtl;
        hltCtl = NULL;
    }
}

void PlayerView::setOpenGlViewport()
{
    this->setViewport(new QOpenGLWidget());
    useOpenGL = true;
}

bool PlayerView::initView(niceXml *reader)
{
    //Set Video Control items
    setXmlSettings(reader);
    setParameters();

    setupDefaultUI();
    initDefaultPlayer();
    initDefaultScene();

//    this->setFullScreen(true);

    if(videoItem != NULL){
        videoItem->setOpenGL(useOpenGL);
        player->setRenderer(videoItem);
        return true;
    }
    else{
        qDebug()<<"nicePlayerView::Renderer not set";
        return false;
    }
}

void PlayerView::setFullScreen(bool st)
{
    if(st){
        qDebug()<<"Player::set to fullscreen mode";
        this->setParent(NULL);
        this->setWindowFlags(this->windowFlags() | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint | Qt::Dialog);
        this->setWindowState(this->windowState() | Qt::WindowFullScreen);
        this->setFocus();
    }else{
        qDebug()<<"Player::Fullscreen mode, set to normal";
        this->setWindowState(this->windowState() & (~Qt::WindowFullScreen) | Qt::WindowMaximized);
    }

    emit fullscreen();
}

void PlayerView::videoRendererClickedSlot()
{
}

void PlayerView::mouseMoveEvent(QMouseEvent *e)
{
//    QPointF pos = e->pos();

//    if(!isFullScreen)
//        showAllControlItems();
//    else
//    {
//        int ypos = (int)pos.y();
//        if(ypos > this->height()*0.75 || ypos < this->height()*0.25)
//            showAllControlItems();
//        else{
//            hideControls();
//            return;
//        }
//    }

    QGraphicsView::mouseMoveEvent(e);
}
void PlayerView::resizeEvent(QResizeEvent *event){

    changeCloseButtonPos();
    qDebug()<<"nicePlayerView::-----Resize Event-----";
    QSizeF viewSize = (this->visibleRect()).size();
    videoItem->resizeRenderer(viewSize.width(), viewSize.height());
    scene->setSceneRect(0,0,viewSize.width(),viewSize.height());
    if(closeButtonEnabled)
        proxyFrame->setGeometry(QRect(proxyFrame->x(),viewSize.height() - (proxyFrame->size()).height(),
                            viewSize.width(), (proxyFrame->size()).height()));
    else{
        proxyFrame->setGeometry(QRect(proxyFrame->x(),proxyFrame->y(),
                            viewSize.width(), (proxyFrame->size()).height()));
    }
    proxyPlaylist->setGeometry(QRect(0,0,viewSize.width(), 110));

    qDebug()<<"Tamaño del Graphics View: "<<viewSize.width()<<" x "<<viewSize.height();
    qDebug()<<"Tamaño de la Escena: "<<scene->width()<<" x "<<scene->height();
    QGraphicsView::resizeEvent(event);
}

void PlayerView::keyPressEvent(QKeyEvent *e)
{

    qDebug()<<"KEY PRESS EVENT FROM PLAYER --> "<<e->key()<<"AUTOREPEAT"<<e->isAutoRepeat();
        switch(e->key()){
        case Qt::Key_Left:
            if(getPlayerState() == QtAV::AVPlayer::PausedState )
                seekFrame(false);

        break;
        case Qt::Key_Right:
            if(getPlayerState() == QtAV::AVPlayer::PausedState )
                seekFrame(true);
        break;
        default:
             break;
        }
        QGraphicsView::keyPressEvent(e);
}

//void nicePlayerView::keyReleaseEvent(QKeyEvent *e){
//    qDebug()<<"KEY RELEASE EVENT FROM PLAYER --> "<<e->key()<<"AUTOREPEAT"<<e->isAutoRepeat();
//    if(e->isAutoRepeat()){
//        e->ignore();
//    }
//    else if(isFullScreen){
//        switch(e->key()){
//        case Qt::Key_Left:{
//            if(getPlayerState() == QtAV::AVPlayer::PlayingState ){
////                emit listKeyReleased();
//                emit arrowClicked(false,false);
//            }
//        }
//            break;
//        case Qt::Key_Right:{
//            if(getPlayerState() == QtAV::AVPlayer::PlayingState ){
//                emit arrowClicked(true,false);
////                 emit listKeyReleased();
//            }
//        }
//            break;
//        }
//    }
//    QGraphicsView::keyReleaseEvent(e);
//}

void PlayerView::mouseReleaseEvent(QMouseEvent *e)
{
    this->setCursor(Qt::PointingHandCursor);
    if(closeFlag)
    {
        //setFullScreen(false);
        emit closeButtonClicked();
        closeFlag = false;
        return;
    }

    QGraphicsView::mouseReleaseEvent(e);
}

void PlayerView::setCloseButtonEnabled(bool value)
{
    closeButtonEnabled = value;
}

void PlayerView::setCurrentIndex(int idx)
{
    if(videoItem != NULL)
        videoItem->setModelIndex(idx);
}

void PlayerView::setRepeat(bool isRepeated)
{
    repeat = isRepeated;
}

qint64 PlayerView::currentPosition()
{
    return player->position();
}

QtAV::AVPlayer::State PlayerView::getPlayerState() const
{
    return playerState;
}
void PlayerView::loadSpeed(qreal speed){
    QString str = QString::number(speed);
    rateBox->findData(str);
    int index = rateBox->findData(str);
    if ( index != -1 ) { // -1 for not found
       rateBox->setCurrentIndex(index);
    }
}

void PlayerView::setXmlSettings(niceXml *value)
{
    xmlSettings = value;
}

void PlayerView::setParameters()
{
    if(xmlSettings != NULL)
    {
//        qDebug() << "Set playerWidget Parameters";
        QDomElement root = xmlSettings->document.firstChildElement();
        QDomElement player = xmlSettings->getClassById(root, "player");

        if(player.isNull())
            qDebug() << "Is Null";
        else{
//            qDebug() << "nicePlayerView::xml parameters file detected";
            xmlSettings->listAllElements(player);

            //notifyInterval
            QString par1 = xmlSettings->getClassTextElement(player,"notifyInterval");
            if(par1 == "NULL_PARAMETER")
                qDebug() << "niceWidget::setParameters::Null Value for 'notifyInterval'' because this does not exist or is repeated, you need to set default parameter";
            else{
//                qDebug()<< "niceWidget::setParameters::notifyInterval :" <<par1;
                m_unit = par1.toInt();
            }
            //initialVolume
            QString par2 = xmlSettings->getClassTextElement(player,"initialVolume");
            if(par2 == "NULL_PARAMETER")
                qDebug() << "niceWidget::setParameters::Null Value for 'initialVolume'' because this does not exist or is repeated, you need to set default parameter";
            else{
//                qDebug()<< "niceWidget::setParameters::initialVolume :" <<par2;
                m_initalVolume = par2.toInt();
            }

            //currentTimeLabelActivated
            QString par3 = xmlSettings->getClassTextElement(player,"currentTimeLabelActivated");
            if(par3 == "NULL_PARAMETER")
                qDebug() << "niceWidget::setParameters::Null Value for 'currentTimeLabelActivated'' because this does not exist or is repeated, you need to set default parameter";
            else{
//                qDebug()<< "niceWidget::setParameters::currentTimeLabelActivated :" <<par3;
                if(par3 == "true")
                    m_currPosViewActivated = true;
                else if(par3 == "false")
                    m_currPosViewActivated = false;
            }
            //tickHeightRatio
            QString par4 = xmlSettings->getClassTextElement(player,"tickHeightRatio");
            if(par4 == "NULL_PARAMETER")
                qDebug() << "niceWidget::setParameters::Null Value for 'tickHeightRatio'' because this does not exist or is repeated, you need to set default parameter";
            else{
//                qDebug()<< "niceWidget::setParameters::tickHeightRatio :" <<par4;
                m_tickHeightRatio = par4.toFloat();
            }
            //tickWidthRatio
            QString par5 = xmlSettings->getClassTextElement(player,"tickWidthRatio");
            if(par5 == "NULL_PARAMETER")
                qDebug() << "niceWidget::setParameters::Null Value for 'tickWidthRatio'' because this does not exist or is repeated, you need to set default parameter";
            else{
//                qDebug()<< "niceWidget::setParameters::tickWidthRatio :" <<par5;
                m_tickWidthRatio = par5.toFloat();
            }
            //timeSliderPos
            QString par6 = xmlSettings->getClassTextElement(player,"timeSliderPos");
            if(par6 == "NULL_PARAMETER")
                qDebug() << "niceWidget::setParameters::Null Value for 'timeSliderPos'' because this does not exist or is repeated, you need to set default parameter";
            else{
//                qDebug()<< "niceWidget::setParameters::timeSliderPos :" <<par6;
                if(par6 == "CENTER")
                    m_timeSliderPos = CENTER_POS;
                else if(par6 == "BOTTOM")
                    m_timeSliderPos = BOTTOM_POS;
                else if(par6 == "TOP")
                    m_timeSliderPos = TOP_POS;
            }
            //frameHeightRatio
            QString par7 = xmlSettings->getClassTextElement(player,"frameHeightRatio");
            if(par7 == "NULL_PARAMETER")
                qDebug() << "niceWidget::setParameters::Null Value for 'frameHeightRatio'' because this does not exist or is repeated, you need to set default parameter";
            else{
//                qDebug()<< "niceWidget::setParameters::frameHeightRatio :" <<par7;
                m_frameHeightRatio = par7.toFloat();
            }
            //previewActivated
            QString par8 = xmlSettings->getClassTextElement(player,"previewActivated");
            if(par8 == "NULL_PARAMETER")
                qDebug() << "niceWidget::setParameters::Null Value for 'previewActivated'' because this does not exist or is repeated, you need to set default parameter";
            else{
//                qDebug()<< "niceWidget::setParameters::previewActivated :" <<par8;
                QDomElement par8_Element = xmlSettings->getClassElement(player,"previewActivated");
                QString par8_Att = par8_Element.attribute("value");
                if(par8_Att == "true")
                    m_previewActivated = true;
                else if(par8_Att == "false")
                    m_previewActivated = false;

                if(m_previewActivated == true)
                {
                    QDomElement previewNode = xmlSettings->getClassElement(par8_Element, "previewActivated");
                    if(!previewNode.isNull())
                    {
                        QString par81= xmlSettings->getClassTextElement(previewNode,"previewHeight");
                        if(par81 == "NULL_PARAMETER")
                            qDebug() << "niceWidget::setParameters::Null Value for 'previewHeight'' because this does not exist or is repeated, you need to set default parameter";
                        else{
//                            qDebug()<< "niceWidget::setParameters::previewHeight :" <<par81;
                            m_previewSize.setHeight(par81.toInt());
                        }
                        QString par82= xmlSettings->getClassTextElement(previewNode,"previewWidth");
                        if(par82 == "NULL_PARAMETER")
                            qDebug() << "niceWidget::setParameters::Null Value for 'previewWidth'' because this does not exist or is repeated, you need to set default parameter";
                        else{
//                            qDebug()<< "niceWidget::setParameters::previewWidth :" <<par82;
                            m_previewSize.setWidth(par82.toInt());
                        }
                        QString par83= xmlSettings->getClassTextElement(previewNode,"previewShowTime");
                        if(par83 == "NULL_PARAMETER")
                            qDebug() << "niceWidget::setParameters::Null Value for 'previewShowTime'' because this does not exist or is repeated, you need to set default parameter";
                        else{
//                            qDebug()<< "niceWidget::setParameters::previewShowTime :" <<par83;
                            m_previewShowTime = par83.toInt();
                        }
                    }else
                        qDebug() << "nicePlayerView::setParameters::previewActivated but no child defined, set default values";
                }
            }
            //defaultPlayBackRate
            QString par9 = xmlSettings->getClassTextElement(player,"defaultPlayBackRate");
            if(par9 == "NULL_PARAMETER")
                qDebug() << "niceWidget::setParameters::Null Value for 'previewActivated'' because this does not exist or is repeated, you need to set default parameter";
            else{

                QDomElement playBackElement = xmlSettings->getClassElement(player, "defaultPlayBackRate");
                QString par9_Att = playBackElement.attribute("value");
//                qDebug()<< "niceWidget::setParameters::previewActivated :" <<par9_Att;
                if(par9_Att == "true")
                    m_defaultPlayBackRate = true;
                else
                    m_defaultPlayBackRate = false;

                if(m_defaultPlayBackRate == false)
                {
                    QDomElement defplaybackNode = xmlSettings->getClassElement(player, "defaultPlayBackRate");
                    if(!defplaybackNode.isNull())
                    {
                        QDomNode rateChild = defplaybackNode.firstChild();
                        while(!rateChild.isNull())
                        {
                            playBackRatesVector.push_back((qreal)rateChild.toElement().text().toFloat());
                            rateChild = rateChild.nextSibling();
                        }
                    }

                    if(playBackRatesVector.size() == 0)
                        m_defaultPlayBackRate = true;
                }
            }

            //enableHideControlFrame
            QString par10 = xmlSettings->getClassTextElement(player,"enableHideControlFrame");
            if(par10 == "NULL_PARAMETER")
                qDebug() << "niceWidget::setParameters::Null Value for 'enableHideControlFrame'' because this does not exist or is repeated, you need to set default parameter";
            else{
//                qDebug()<< "niceWidget::setParameters::enableHideControlFrame :" <<par10;
                if(par10 == "true")
                    m_enableHideControlFrame = true;
                else if(par10 == "false")
                    m_enableHideControlFrame = false;
            }
        }
    }
}

void PlayerView::initPlayerWidget(const QString &path){
    qDebug()<<"initPlayerWidget";
    if(path != "NULL"){
        this->setPath(path);
        QString argument = this->path;


        if(this->openMedia(argument)){
            qDebug()<< "nicePlayerView:: Video load OK";

        }else
            qDebug()<< "nicePlayerView:: Video load Error";
    }else
        qDebug()<<"nicePlayerView:: Empty Video Data";
}

void PlayerView::setupDefaultUI(){
    frame = new BorderFrame();
    //frame->setAttribute(Qt::WA_TranslucentBackground);
    QFont font = QFont("Frutiger CE 45 Light",50);
    frame->setFont(font);
    frame->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed));
    frame->setStyleSheet("QFrame{"
                         "background-color: qlineargradient(spread:pad, x1:0 y1:0, x2:0 y2:1, stop:0 rgba(255, 255, 255, 0), stop:1 rgba(0, 0, 0, 100) );"
                         "}"
                         );
    playlistFrame = new sliceFrame();
    QSize size1(51,51); //Close, Fordward, Backward Buttons
    QSize size2(23,23); //play, volume, rate, photo, max, edit buttons

    closeButton = new CustomPushButton();
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setFlat(true);
    closeButton->setFont( QFont("Frutiger CE 45 Light",9) );
    closeButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
    closeButton->setFixedSize(size1);
    closeButton->setStyleSheet("QPushButton{"
                               "background-color: transparent;"
                               "border-image: url(:/resources/close.png);}"
                               "QPushButton:hover{"
                               "border-image: url(:/resources/close-hover.png);"
                               "}");

    this->initButton(&playButton, size2,"QPushButton{"
                                        "background-color: transparent;"
                                        "border-image: url(:/resources/play2.png);}"
                                        "QPushButton:checked{"
                                        "border-image: url(:/resources/stop.png);}"
                                        "QPushButton:hover{"
                                        "background-color: transparent;"
                                        "border-image: url(:/resources/play2-activo.png);}"
                                        "QPushButton:checked:hover{"
                                        "border-image: url(:/resources/stop-activo.png);}"
                                        "QPushButton:disabled{"
                                        "background-color: transparent;"
                                        "border-image: url(:/player/resource/niceplayer/play2-inhabilitado.png);}"
                                        "QPushButton:checked:disabled{"
                                        "border-image: url(:/player/resource/niceplayer/stop-inhabilitado.png);}", frame, true);
    playButton->setChecked(false);
    //Volume
    this->initButton(&volumeButton, size2, "QPushButton{"
                                           "background-color: transparent;"
                                           "border-image: url(:/resources/volumen.png);}"
                                           "QPushButton:checked{"
                                           "background-color: transparent;"
                                           "border-image: url(:/resources/Media-Controls-Mute-icon.png);}"
                                           "QPushButton:hover{"
                                           "background-color: transparent;"
                                           "border-image: url(:/resources/volumen-activo.png);}"
                                           "QPushButton:checked:hover{"
                                           "background-color: transparent;"
                                           "border-image: url(:/resources/icono-de-silencio-55100.png);}"
                                           "QPushButton:disabled{"
                                           "background-color: transparent;"
                                           "border-image: url(:/resources/volumen-inhabilitado.png);}", frame, true);

    this->initButton(&maxButton, size2, "QPushButton {"
                                                    "background-color: transparent;"
                                                    "border-image: url(:/resources/maximizar.png)}"
                                                    "QPushButton:hover { border-image: url(:/resources/maximizar-activo.png)}", frame, true);
    maxButton->setFocusPolicy(Qt::NoFocus);
    //Volume Slider
    volumeSlider = new NiceSlider(Qt::Horizontal);
    volumeSlider->setObjectName("volumeSlider");
    volumeSlider->setCursor(Qt::PointingHandCursor);
    volumeSlider->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
    volumeSlider->setFixedSize(QSize(60,33));

    //Volume Settings
    prevVolumenPos = m_initalVolume;
    volumeSlider->setValue(m_initalVolume);
    volumeSlider->setRange(0,100);
    volumeSlider->setStyleSheet("QSlider{"
                                "background: transparent;}"

                                "QSlider#volumeSlider::groove:horizontal{"
                                "background: grey;"
                                "height: 2px;"
                                "border-radius: 1px;}"

                                "QSlider#volumeSlider::handle:horizontal{"
                                "background: white;"
                                "height: 8px;"
                                "width: 8px;"
                                "border: 1px transparent;"
                                "border-radius: 4px;"
                                "margin: -3px 0;}"

                                "QSlider#volumeSlider::handle:horizontal:hover{"
                                "background: rgba(214, 174, 10, 255);"
                                "height: 8px;"
                                "width: 8px;"
                                "border: 1px transparent;"
                                "border-radius: 4px;"
                                "margin: -3px 0;}"

                                "QSlider#volumeSlider::sub-page:horizontal{"
                                "background: rgb(241, 196, 13);}"

                                "QSlider#volumeSlider::add-page:horizontal{"
                                "background: grey;}"
                                );
    //Curr Pos
    currentPosLabel = new QLabel(frame);
    QFont frameTextFont = QFont("Frutiger CE 45 Light",9);
    currentPosLabel->setFont(frameTextFont);
    currentPosLabel->setObjectName("positionLabel");
    currentPosLabel->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
    currentPosLabel->setStyleSheet("QLabel#positionLabel{"
                                   "background-color: rgba(26,26,26,100);"
                                   "color: white;"
                                   "border-right: 1px solid qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0.221591 rgba(0, 0, 0, 200), stop:0.994318 rgba(30, 30, 30, 200), stop:1 rgba(30, 30, 30, 100));"
                                   "border-bottom: 2px solid qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0.221591 rgba(0, 0, 0, 200), stop:0.994318 rgba(30, 30, 30, 200), stop:1 rgba(30, 30, 30, 100));"
                                   "border-left: 2px rgba(0,0,0,255);"
                                   "border-radius: 2px;"
                                   "padding-top: 3px;"
                                   "padding-right: 4px;}"
                                   );
    QString initPosStr = " 00:00/00:00 ";
    QFontMetrics mtr(frameTextFont);
    int strWide = mtr.width(initPosStr);
    int strHeight = mtr.height();
    currentPosLabel->setText(initPosStr);
    currentPosLabel->setFixedSize(QSize(strWide,strHeight + 2));

//    Time Slider
    timeSlider = new NiceTimeSlider(Qt::Horizontal, frame);
    timeSlider->setObjectName("timeSlider");
    timeSlider->setCursor(Qt::PointingHandCursor);
    timeSlider->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    timeSlider->setMinimumSize(QSize(50,33));
    timeSlider->setMaximumSize(QSize(24580,33));
    //timeSlider->setMinimum(0);
    //timeSlider->setMaximum(1000);
    timeSlider->setValue(0);
    timeSlider->setStyleSheet(  "QSlider{"
                                "background: transparent;}"

                                "QSlider::groove:horizontal{"
                                "background: grey;"
                                "height: 2px;"
                                "border: 1px transparent;"
                                "border-radius: 1px;}"

                                "QSlider::handle:horizontal{"
                                "height: 8px;"
                                "width: 3px;"
                                "background:  white;"
                                "border: 1px transparent;"
                                "border-radius: 1px;"
                                "margin: -3px 0;}"

                                "QSlider::handle:horizontal:hover{"
                                "height: 8px;"
                                "width: 3px;"
                                "background:  rgba(214, 174, 10, 255);"
                                "border: 1px transparent;"
                                "border-radius: 1px;"
                                "margin: -3px 0;}"

                                "QSlider::add-page:horizontal{"
                                "background: rgb(200, 200, 200);}"

                                "QSlider::sub-page:horizontal{"
                                "background: rgb(241, 196, 13);"
                                "border: 1px solid rgba(214, 174, 10, 200);}"
                                );

    //Setting timeslider tick height and width
    timeSlider->updateTickGeometry(m_tickWidthRatio, m_tickHeightRatio);

    //Rate Control
    rateBox = new upWardComboBox(frame);
    rateBox->setCursor(Qt::PointingHandCursor);
    rateBox->setObjectName("rateBox");
    rateBox->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
    rateBox->setFixedSize(QSize(50,18));

    rateBox->setStyleSheet("QComboBox {"
                           "background-color: rgba(26,26,26,100);"
                           "color: white;"
                           "border-right: 2px solid qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0.221591 rgba(0, 0, 0, 200), stop:0.994318 rgba(30, 30, 30, 200), stop:1 rgba(30, 30, 30, 100));"
                           "border-bottom: 2px solid qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0.221591 rgba(0, 0, 0, 200), stop:0.994318 rgba(30, 30, 30, 200), stop:1 rgba(30, 30, 30, 100));"
                           "border-left: 1px rgba(0,0,0,255);"
                           "border-radius: 2px;"
                           "padding-top: 3px;"
                           "padding-left: 4px;}"

                           "QComboBox:hover{"
                           "color: rgba(214, 174, 10, 255);}"

                           "QComboBox::drop-down {"
                           "subcontrol-origin: padding;"
                           "subcontrol-position: top right;"
                           "width: 10px;"
                           "border: none;}"

                           "QComboBox::down-arrow {"
                           "image: url(:/resources/dropdown.png);"
                           "width: 10px;"
                           "height: 8px;}"

                           "QComboBox::down-arrow:hover {"
                           "image: url(:/resources/dropdown-activo.png);"
                           "width: 10px;"
                           "height: 8px;}"

                           "QComboBox::down-arrow:on {"
                           "image: url(:/resources/dropdown-activo.png);"
                           "width: 10px;"
                           "height: 8px;}"

                           "QComboBox QAbstractItemView {"
                           "border: none;"
                           "selection-background-color: transparent;"
                           "background-color: rgb(26, 26, 26);"
                           "outline: none;}"

                           "QComboBox QAbstractItemView::item {"
                           "color: white;"
                           "height: 15px;"
                           "border: 1px solid grey;"
                           "outline: none;}"

                           "QComboBox QAbstractItemView::item:hover {"
                           "color: rgb(241, 196, 13);"
                           "outline: none;}"

                           "QComboBox:Disabled {border-image: url(:/resources/boton-velocidad-inhabilitado.png);"
                           "width: 10px;"
                           "height: 8px;"
                           "padding: 0 5px 5px 20px;}"
                           );

    rateBox->setView(new QListView());
    if(m_defaultPlayBackRate || playBackRatesVector.size() == 0){
        rateBox->addItem("0.25", QVariant(0.25));
        rateBox->addItem("0.5", QVariant(0.5));
        rateBox->addItem("0.75", QVariant(0.75));
        rateBox->addItem("1.0", QVariant(1.0));
        //lunes: quitar o remover velocidades
        rateBox->addItem("2.0", QVariant(2.0));
        rateBox->addItem("4.0", QVariant(4.0));
        rateBox->setCurrentIndex(3);
    }else
    {
        QVector<qreal>::iterator it;
        for(it = playBackRatesVector.begin(); it != playBackRatesVector.end(); it++){
            QString n;
            if(*it >= 1.0)
                n = QString::number(*it) + ".0";
            else
                n = QString::number(*it);

            rateBox->addItem(n, QVariant(*it));
        }

    }
    l = new QHBoxLayout();
    l->addWidget(playButton);
    l->addWidget(volumeButton);
    l->addWidget(volumeSlider);
    if(m_currPosViewActivated)
        l->addWidget(currentPosLabel);

    if(m_timeSliderPos != CENTER_POS){
        sp1 = new QSpacerItem(10,frame->width()*0.5,QSizePolicy::Maximum, QSizePolicy::Minimum);
        optL = new QVBoxLayout();
        l->addItem(sp1);
        l->setSpacing(10);
    }
    else{
        sp1 = new QSpacerItem(10,20,QSizePolicy::Fixed, QSizePolicy::Minimum);
        sp2 = new QSpacerItem(10,20,QSizePolicy::Fixed, QSizePolicy::Minimum);
        l->addItem(sp1);
        l->addWidget(timeSlider);
        l->addItem(sp2);
    }

    l->addWidget(rateBox);
    l->addWidget(maxButton);

    if(m_timeSliderPos != CENTER_POS){
        if(m_timeSliderPos == TOP_POS){
            optL->addWidget(timeSlider);
            optL->addItem(l);
        }else if(m_timeSliderPos == BOTTOM_POS)
        {
            optL->addItem(l);
            optL->addWidget(timeSlider);
        }
    }

    frame->setMinimumWidth(this->width());

    if(optL != NULL){
        frame->setFixedHeight(frame->height()*1.8);
        frame->setLayout(optL);
    }
    else
        frame->setLayout(l);
}

void PlayerView::toggleCloseButtonStyleSheet(bool largeStyle)
{
    if(closeButton != NULL)
    {
        freeWidget(closeButton);
        delete closeButton;
    }

    closeButton = new CustomPushButton();
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setFlat(true);
    closeButton->setFont( QFont("Frutiger CE 45 Light",9) );
    closeButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));

    if(largeStyle){

        closeButton->setFixedSize(101,33);
        closeButton->setBorder(4);
        closeButton->setText("Return");
        closeButton->setStyleSheet("QPushButton{"
                                    "background-color: rgb(241,196,13);"
                                    "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0  rgb(191,155,10), stop: 1  rgb(254,216,58));"
                                    "}"
                                    "QPushButton::hover{"
                                    "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0  rgb(239,197,26), stop: 1  rgb(254,216,102));"
                                    "}");
    }else
    {
        closeButton->setFixedSize(51,51);
        closeButton->setBorder(0);
        closeButton->setText("");
        closeButton->setStyleSheet("QPushButton{"
                                   "background-color: transparent;"
                                   "border-image: url(:/resources/close.png);}"
                                   "QPushButton:hover{"
                                   "border-image: url(:/resources/close-hover.png);"
                                   "}");

    }

    proxyCloseButton = scene->addWidget(closeButton);
    connect(closeButton,SIGNAL(clicked()),this,SLOT(closeButtonClickedSlot()));
    this->changeCloseButtonPos();
}

void PlayerView::enableTimeSlider(){
    timeSlider->setEnabled(true);
}

void PlayerView::disableTimeSlider(){
    //this->player->pause(true);
    timeSlider->setEnabled(false);
}

void PlayerView::initButton(QPushButton **btn, QSize size, const QString &style, QWidget *parent, bool checkeable)
{
    (*btn) = new QPushButton(parent);
    (*btn)->setCursor(Qt::PointingHandCursor);
    (*btn)->setFlat(true);
    if(checkeable)
        (*btn)->setCheckable(checkeable);
    (*btn)->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
    (*btn)->setFixedSize(size);
    if(style.contains("QPushButton"))
        (*btn)->setStyleSheet(style);
    else{
        (*btn)->setFlat(false);
        (*btn)->setAutoFillBackground(true);
        (*btn)->setAutoDefault(true);
        (*btn)->setText(style);
    }
    (*btn)->setFocusPolicy(Qt::NoFocus);
}

void PlayerView::initDefaultPlayer(){
    m_previewSize = QSize(100,80);
    //Init Video Item
    //videoItem = new QtAV::GraphicsItemRenderer();
    videoItem = new niceDragItemRenderer;

    if(!videoItem->graphicsItem()){
        QMessageBox::warning(0, QString::fromLatin1("QtAV error"), tr("Can not create video renderer"));
        return;
    }

//    videoItem->resizeRenderer(this->size().width()-1,this->size().height()-1);
    videoItem->setOutAspectRatioMode(QtAV::VideoRenderer::VideoAspectRatio);

    //item->setFlag(QGraphicsItem::ItemIsMovable);
    //item->setPos(0,0);

    //Init Player
    player = new QtAV::AVPlayer(this);
    player->setAsyncLoad(true);
    player->setSeekType(QtAV::AccurateSeek);
    player->audio()->setVolume(m_initalVolume*0.01);
    player->setNotifyInterval(m_unit);
    timeSlider->setUseTicks(false);
    connect(videoItem, SIGNAL(videoRendererClicked()),this, SLOT(videoRendererClickedSlot()));

    connect(timeSlider, SIGNAL(sliderMoved(int)), SLOT(seekBySlider(int)));
    connect(timeSlider, SIGNAL(onHover(int,int)), SLOT(onTimeSliderHover(int,int)));
    connect(timeSlider, SIGNAL(leave()), SLOT(onTimeSliderLeave()));

    connect(player, SIGNAL(positionChanged(qint64)), SLOT(updateSlider(qint64)));
    connect(player, SIGNAL(notifyIntervalChanged()), SLOT(updateSliderUnit()));
    connect(player, SIGNAL(mediaStatusChanged(QtAV::MediaStatus)),
             this, SLOT(statusChanged(QtAV::MediaStatus)));
    //connect(player, SIGNAL(videoAvailableChanged(bool)), this, SLOT(videoAvailableChanged(bool)));
//    connect(player, SIGNAL(error(QtAV::AVError)), this, SLOT(displayErrorMessage(QtAV::AVError)));
    connect(player, SIGNAL(stateChanged(QtAV::AVPlayer::State)),
           this, SLOT(setState(QtAV::AVPlayer::State)));
    connect(this, SIGNAL(play()), this, SLOT(playPause()));
    connect(this, SIGNAL(pause()), this, SLOT(playPause()));
    connect(this, SIGNAL(playForced()), this, SLOT(playForcedSlot()));
    connect(playButton,SIGNAL(clicked()),this,SLOT(managePlayPauseStatus()));
    connect(volumeSlider,SIGNAL(valueChanged(int)),this,SLOT(volumeSliderMoved(int)));
    connect(volumeButton,SIGNAL(toggled(bool)),this,SLOT(volumeButtonClicked(bool )));
//    connect(volumeButton,SIGNAL(hovered()),volumeSlider,SLOT(show()));
    //RateBox settings
    connect(rateBox, SIGNAL(currentIndexChanged(int)),this,SLOT(updateRate()));

    connect(maxButton,SIGNAL(clicked()),this,SIGNAL(fullscreen()));
}

void PlayerView::initDefaultScene()
{

    //Init Scene
    connect(&previewTimer, SIGNAL(timeout()),this,SLOT(hidePreview()));

    scene = new QGraphicsScene(0,0,(this->size()).width(),(this->size()).height());
    setScene(scene);
    scene->addItem(videoItem);
    proxyPlaylist = scene->addWidget(playlistFrame);

    if(closeButton != NULL){
        proxyCloseButton = scene->addWidget(closeButton);
        connect(closeButton,SIGNAL(clicked()),this,SLOT(closeButtonClickedSlot()));
    }

    if(frame != NULL){
        proxyFrame = scene->addWidget(frame);
        //proxyFrame->setPos(0,visibleRect().height() - (proxyFrame->size()).height());
        proxyFrame->hide();
    }else qDebug()<<"Control Frame not defined";

    timeSlider->setEnabled(false);
}
void PlayerView::setTrackPosition(qint32 start, qint32 end){
    startTrackPos = (qint64)start;
    endTrackPos = (qint64)end;
}
void PlayerView::seekToEvent(){
      qDebug()<<"SEEKTOEVENT";
      sliderLimitsChangeSlot(startTrackPos,endTrackPos);
      player->seek(startTrackPos);
//      connect(this,SIGNAL(initPlayer(qint64)),player,SLOT(seek(qint64)));
      emit playForced();
}
void PlayerView::stopPlayer()
{
    if(player != NULL && player->isPlaying())
        player->stop();

    timeSlider->setValue(0);
}

void PlayerView::statusChanged(QtAV::MediaStatus status)
{

    // handle status message
    switch (status)
    {
    case QtAV::UnknownMediaStatus:
        qDebug()<<"Unknown Media Status";
        break;
    case QtAV::NoMedia:
        qDebug()<<"No Media Status";
        break;
    case QtAV::LoadedMedia:{

        qDebug()<<"Loaded Media"<<player->duration();
        this->setPath(player->file());
        this->enableTimeSlider();
        if(startTrackPos == -1){
            sliderLimitsChangeSlot(0,player->duration());
        }
        else if(player->duration() != 0){
            sliderLimitsChangeSlot(startTrackPos,endTrackPos);
        }
        //videoItem->setSpeed(this->playbackRate());

        emit videoLoaded();
    }
        break;
    case QtAV::BufferingMedia:
        qDebug()<<"Buffering Media";
        break;
    case QtAV::BufferedMedia:
        qDebug()<<"Buffered Media";
        break;
    case QtAV::LoadingMedia:
        qDebug()<<"Loading Media";
        break;
    case QtAV::StalledMedia:
        qDebug()<<"Media Stalled";
        break;
    case QtAV::EndOfMedia:
        qDebug()<<"End of Media";
        break;
    case QtAV::InvalidMedia:
        displayErrorMessage(QtAV::AVError::FormatError);
        break;
    }

    //    qDebug()<<"-----------------------------------------------";
}

void PlayerView::sliderLimitsChangeSlot(qint64 init, qint64 end)
{
    qDebug()<<"sliderLimitsChange"<<"init"<<init<<"end"<<end<<"duration"<<player->duration();
    if(init >= 0 && end <= player->duration() && init < end){
        qDebug()<<"nicePlayerView::view zone changed to: "<< init<< " - "<< end;
        if(setSliderLimitTimes(init,end)){
            timeSlider->setRange(0, int((endTime-startTime)/m_unit));
            timeSlider->setExtUnit(m_unit);
            timeSlider->setExtInitValue(startTime);
            player->setStartPosition(startTime);
        }else
            qDebug()<<"sliderLimitsChange::setSliderLimitTimes error! ";
    }
}
void PlayerView::displayErrorMessage(QtAV::AVError n_error)
{
    if (n_error.error() != 0){

        if(n_error.error()==9)timeSlider->setValue(timeSlider->maximum());
            else{
            QMessageBox messageBox;
            messageBox.critical(0,"Error Number: ",QString::number(n_error.error()));
            messageBox.setFixedSize(500,200);
        }
    }
}

void PlayerView::setState(QtAV::AVPlayer::State state)
{
    qDebug()<<"nicePlayerView::Player State Changed to ";
    if (state != playerState)
        playerState = state;

    switch (state) {
    case QtAV::AVPlayer::StoppedState:{
        qDebug()<<"Stop State";
        if(!player->isPlaying()){
            timeSlider->setValue(timeSlider->minimum());
            playButton->setChecked(false);
//            if(startTrackPos != -1)
//                connect(this,SIGNAL(initPlayerSignal(qint64)),player,SLOT(seek(qint64)));
//            else{
//                    player->setPosition(startTime);
//            }
        }
    }
        break;
    case QtAV::AVPlayer::PlayingState:{
        qDebug()<<"Playing State at position: "<<startTime;
        //        player->setSeekType(QtAV::AccurateSeek);
//        emit(initPlayerSignal(startTime));
//        disconnect(this,SIGNAL(initPlayerSignal(qint64)),player,SLOT(seek(qint64)));
        playButton->setChecked(true);
//        player->setSeekType(QtAV::KeyFrameSeek);
    }
        break;
    case QtAV::AVPlayer::PausedState:{
        qDebug()<<"Paused State";
        playButton->setChecked(false);
    }
        break;
    default:
        qDebug()<<"Default State";
        break;
    }

}

void PlayerView::updateRate()
{
    qreal rate = rateBox->itemData(rateBox->currentIndex()).toDouble();
    player->setSpeed(rate);
}

void PlayerView::volumeSliderMoved(int position)
{
    qDebug()<<"Player::Setting Volume to "<<position;
    player->audio()->setVolume(position/100.0);
    if(position <= 0)
        volumeButton->setChecked(true);
    else
        volumeButton->setChecked(false);
}

void PlayerView::managePlayPauseStatus(){
    qDebug()<< "Player::manage play/pause status :: state"<<playerState;
    switch (playerState) {
    case QtAV::AVPlayer::StoppedState:
        emit play();
    case QtAV::AVPlayer::PausedState:
        emit play();
        break;
    case QtAV::AVPlayer::PlayingState:{
        emit pause();
    }
        break;
    default:
        qDebug()<< "Player::Play/Pause_Warning: No State for Player";
    }
}

void PlayerView::volumeButtonClicked(bool st)
{
    qDebug()<<"Volume button toggled: "<<st;
    if(st == true){
        prevVolumenPos = volumeSlider->value();
        volumeSlider->setValue(0);
    }
    else
    {
        if(prevVolumenPos > 0)
            volumeSlider->setValue(prevVolumenPos);
        else
            volumeSlider->setValue((volumeSlider->maximum()-volumeSlider->minimum())/2);

    }
}

bool PlayerView::openMedia(QString path)
{
    qDebug()<<"nicePlayerView::File -> "<<path;
//    player->stop();
    QString file = path;
    if (file.isEmpty())
        return false;
    player->play(file);

    if (!m_preview && m_previewActivated && this->isEnabled()){
        m_preview = new QtAV::VideoPreviewWidget();
//        m_preview->setStyleSheet("QWidget { "
//                                 "background-color: transparent;"
//                                 "border-width: 3px;"
//                                 "border-radius: 5px white;}"
//                                 );
        proxyPreviewWidget = scene->addWidget(m_preview);
        proxyPreviewWidget->hide();
    }

    if(m_previewActivated && this->isEnabled())
        m_preview->setFile(file);
    return true;
}

void PlayerView::seekBySlider(int value)
{
    if (!player->isPlaying())
        return;

    player->seek(qint64(value*m_unit) + startTime);
    /*Si preview esta habilitado*/
    if (!m_preview && !m_previewActivated)
            return;

    m_preview->setTimestamp(qint64(value*m_unit) + startTime);
    m_preview->preview();
    m_preview->setWindowFlags(m_preview->windowFlags() |Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    m_preview->resize(m_previewSize);
    m_preview->show();
    previewTimer.stop();
    previewTimer.start(m_previewShowTime);
}

void PlayerView::seekBySlider()
{
    seekBySlider(timeSlider->value());
}

void PlayerView::playPause()
{
    if (!player->isPlaying()) {
        qDebug()<<"nicePlayerView::play emited from playPause()";
        player->play();
        return;
    }
    qDebug()<<"nicePlayerView::pause emited from playPause()";
    player->pause(!player->isPaused());
}

void PlayerView::playForcedSlot()
{
    if (!player->isPlaying()) {
        qDebug()<<"nicePlayerView::play emited from playForcedSlot()";
        player->play();
        return;
    }
}

void PlayerView::closeButtonClickedSlot()
{
    qDebug()<<"nicePlayerView::Close Button Clicked";
    //emit closeButtonClicked();
    closeFlag = true;
}

void PlayerView::setCurrTimeView(qint64 value)
{
    Q_UNUSED(value)
//    if(value == 0)return;
    QString currText = currentPosLabel->text();
    QStringList splitList = currText.split('/');
    QString currTimeString = "X";
    QString durTimeString = "X";

    if(splitList.size() == 2){
        currTimeString = splitList[0];
        durTimeString = splitList[1];
    }

    QTime durTime = QTime(0, 0, 0).addMSecs(endTime-startTime);

    QString st;
    if(durTime.hour() == 0){
//        if(durTime.minute() == 0)
//            st = durTime.toString(QString::fromLatin1("ss"));
//        else
            st = durTime.toString(QString::fromLatin1("mm:ss"));
    }else
        st = durTime.toString(QString::fromLatin1("HH:mm:ss"));

    durTimeString = st != durTimeString ? st : durTimeString;
    if(currentPosition()-startTime < 0 )return;
    QTime currTime = QTime(0, 0, 0).addMSecs(currentPosition()-startTime);

    if(durTime.hour() == 0){

//        if(durTime.minute() == 0)
//            st = currTime.toString(QString::fromLatin1("ss"));
//        else
            st = currTime.toString(QString::fromLatin1("mm:ss"));
    }else
        st = currTime.toString(QString::fromLatin1("HH:mm:ss"));

    currTimeString = st != currTimeString ? st : currTimeString;
    currentPosLabel->setText(currTimeString+'/'+durTimeString);

}

void PlayerView::updateSlider(qint64 value)
{
    int newSliderValue = ((int)(value - startTime))/m_unit;

    if(newSliderValue == timeSlider->value())
        return;

//    qDebug()<<"Update Slider";
//    qDebug()<<"---------------Temporal Info------------";
//    qDebug()<<"New Value = "<<value;
//    qDebug()<<"End Time = "<<endTime;
//    qDebug()<<"---------------Slider Info---------------";
//    qDebug()<<"New Slider value"<< newSliderValue;
//    qDebug()<<"Current Slider Value"<<timeSlider->value();

    if( value > endTime ){
        //player->seek((qint64)startTime);
        qDebug()<<"Video end reached";
        if(repeat){
            player->seek(startTime);
            timeSlider->setValue(timeSlider->maximum());
        }else{
            emit endCurrentMediaItem(true);
            return;
        }
    }else{

        if( (startTimeROI > -1 && endTimeROI > -1) && (value < startTimeROI || value > endTimeROI ) )
            player->seek( startTimeROI );

        timeSlider->setValue(newSliderValue);
    }

    setCurrTimeView(value);

    emit positionChanged_hltView(value);
}

void PlayerView::updateSliderUnit()
{
    qDebug()<<"nicePlayerView::Update Slider Unit";
    m_unit = player->notifyInterval();
    timeSlider->setRange(0, int((endTime-startTime)/m_unit));
    timeSlider->setExtUnit(m_unit);
    timeSlider->setExtInitValue(startTime);
}

void PlayerView::showControlFrame()
{
    if(proxyFrame != NULL && !proxyFrame->isVisible())
        proxyFrame->show();
}

void PlayerView::showBasicControlItems()
{
    if(!playButton->isVisible())
        playButton->show();

    if(volumeButton->isVisible())
        volumeButton->hide();

    if(volumeSlider->isVisible())
        volumeSlider->hide();

    if(currentPosLabel->isVisible())
        currentPosLabel->hide();

    if(timeSlider->isVisible())
        timeSlider->hide();

    if(rateBox->isVisible())
        rateBox->hide();

}

void PlayerView::showAdvancedControlItems()
{
    if(!volumeButton->isVisible())
        volumeButton->show();

    if(!volumeSlider->isVisible())
        volumeSlider->show();

    if(!currentPosLabel->isVisible())
        currentPosLabel->show();

    if(!timeSlider->isVisible())
        timeSlider->show();

    if(!rateBox->isVisible())
        rateBox->show();

    if(closeButton != NULL && !closeButton->isVisible())
        closeButton->show();
}


void PlayerView::deleteProxyWidgets()
{

    if(closeButton != NULL)
    {
        freeWidget(closeButton);
        delete closeButton;
    }

    if(playlistFrame != NULL)
    {
        freeWidget(playlistFrame);
        delete playlistFrame;
    }

    if(m_preview != NULL){
        freeWidget(m_preview);
        delete m_preview;
    }

    if(frame != NULL){
        freeWidget(frame);
        delete frame;
    }


}

void PlayerView::freeWidget(QWidget *foo)
{
    const auto proxy = (foo)->graphicsProxyWidget();
    if(proxy){
        scene->removeItem(proxy);
        proxy->setWidget(nullptr);
        proxy->deleteLater();
    }

    (foo)->setParent(nullptr);
}

void PlayerView::changeCloseButtonPos()
{
    if(proxyCloseButton != NULL){
        int margin = 15;
        if(closeButton->text().isEmpty())
            margin = 0;
        QSizeF viewSize = this->visibleRect().size();
        qreal x,y = 0;
        //QSizeF buttonSize = proxyCloseButton->size();
        QSizeF buttonSize( closeButton->size() );
        x = viewSize.width() - (qreal)buttonSize.width() - margin;

        if(playlistFrame->getMyWidget() != NULL && playlistFrame->getMyWidget()->isVisible())
            y = proxyPlaylist->size().height();

        proxyCloseButton->setGeometry(QRectF(QPointF(x,y),buttonSize));
    }
}

void PlayerView::setupTopWidget(QWidget *w, bool useMoveButtons)
{
    this->playlistFrame->insertCenterWidget(w, useMoveButtons);
    playlistFrame->showButtonClicked(false);
}

void PlayerView::hideControls()
{
    if(proxyFrame != NULL){
        if(proxyFrame->isVisible() && m_enableHideControlFrame){
            proxyFrame->hide();
        }
    }

    if(m_preview != NULL){
        if(m_preview->isVisible())
            m_preview->hide();
    }

    if(toolTipItem != NULL){
        if(toolTipItem->isVisible())
            toolTipItem->hide();
    }
}

void PlayerView::onTimeSliderHover(int pos, int value)
{
    if(!timeSlider->isVisible())
        return;
    //QPoint gpos = mapToGlobal(QPoint(timeSlider->pos().x(),0) + QPoint(pos, proxyFrame->y()));q
    //QToolTip::showText(gpos, QTime(0, 0, 0).addMSecs(value).toString(QString::fromLatin1("HH:mm:ss")));
    QString text = QTime(0, 0, 0).addMSecs(value).toString(QString::fromLatin1("HH:mm:ss"));
    if(!toolTipItem){
        toolTipItem = scene->addText("<div style='background-color:transparent; color:white;'>" + text + "</div>");
    }
    else
        toolTipItem->setHtml("<div style='background-color:transparent; color:white;'>" + text + "</div>");
        //toolTipItem->setPlainText(QTime(0, 0, 0).addMSecs(value).toString(QString::fromLatin1("HH:mm:ss")));

    QPoint cpos = QPoint(timeSlider->pos().x(),0) + QPoint(pos, proxyFrame->y());

    toolTipItem->setPos(cpos-QPoint(toolTipItem->boundingRect().width()/2,0));
    toolTipItem->setZValue(1.0);
    toolTipItem->show();

    if (!m_previewActivated)
            return;

    if (!m_preview){
        m_preview = new QtAV::VideoPreviewWidget(this);
        proxyPreviewWidget = scene->addWidget(m_preview);
    }

    m_preview->setFile(player->file());
    m_preview->setTimestamp(value);
    m_preview->preview();
    const int w = m_previewSize.width();
    const int h = m_previewSize.height();
    m_preview->setWindowFlags(m_preview->windowFlags() |Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    m_preview->resize(w, h);
    proxyPreviewWidget->setPos(cpos - QPointF(((qreal)w)/2.0, (qreal)h));
    m_preview->show();
    previewTimer.stop();
    previewTimer.start(m_previewShowTime);
}

void PlayerView::onTimeSliderLeave()
{
    //Hide Preview and Text with Time Values
    if (m_preview && m_preview->isVisible())
            m_preview->hide();

    if (toolTipItem && toolTipItem->isVisible())
        toolTipItem->hide();

    enteredTickSlot(-1);
    timeSlider->setTickHovered(-1);
}

void PlayerView::showToolTipExt(int value)
{
    //qDebug()<<"nicePlayerView::Change Tool Tip to:  "<<value;
    int position = timeSlider->timeToSliderPos((qreal)value);
    onTimeSliderHover(position,value);
}

void PlayerView::selectTick(int idx)
{
    timeSlider->setTickSelected(idx);
    //timeSlider->setSpeed(playbackRate());
}

void PlayerView::hoveredTick(int idx)
{
    timeSlider->setTickHovered(idx);
}

void PlayerView::enteredTickSlot(int idx)
{
    emit tickEntered(idx);
}

void PlayerView::seekFrame(bool isForward){
   qint64 pos = player->position();
   qDebug()<<"OLD:"<<pos;
    if(isForward)
        player->stepForward();
    else
        player->stepBackward();
}

void PlayerView::hidePreview()
{
    if(m_preview->isVisible())
        this->m_preview->hide();
}

void PlayerView::setPath(const QString &value)
{
    path = value;
    this->timeSlider->setFirstDragParam(path);
    this->videoItem->setPath(path);
}

void PlayerView::selectSlot(int index)
{
    //Select in this view
    this->selectTick(index);
}

bool PlayerView::setSliderLimitTimes(qint32 m_startTime, qint32 m_endTime)
{
    //Si los límites no cumples restricciones, se asignan a la posición inicial y final del player
    if(player->isLoaded()){
        if(m_startTime > 0)
            startTime = m_startTime;
        else
            startTime = 0;

        if(m_endTime < player->duration() && m_endTime > 0)
            endTime = m_endTime;
        else
            endTime = player->duration();
        qDebug() << "nicePlayerView:: Limit values ok";

        return true;
    }else{
        qDebug() << "nicePlayerView:: Set limit time values error, no video file loaded";
        return false;
    }
}

QRectF PlayerView::visibleRect() {
    QPointF tl(horizontalScrollBar()->value(), verticalScrollBar()->value());
    QPointF br = tl + viewport()->rect().bottomRight();
    QMatrix mat = matrix().inverted();
    return mat.mapRect(QRectF(tl,br));
}
