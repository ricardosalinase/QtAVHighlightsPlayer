#include <QDebug>
#include <QListWidget>
#include <QFont>
#include "hltplayerctl.h"
#include "PlayerView.h"
#define THUMBSLIDER_WIDTH 100
#define THUMBSLIDER_HEIGHT 75


HltPlayerCtl::HltPlayerCtl() :
    thumbHeight(THUMBSLIDER_HEIGHT)
  , thumbWidth(THUMBSLIDER_WIDTH)
  , hoverColor(29,29,27,255)
  , textColor(255,255,255,255)
{
    pv = new PlayerView;
    //Setup list view of items
    setupListView();
    //Set Player
    setupPlayer();
    pv->setOpenGlViewport();
}

HltPlayerCtl::~HltPlayerCtl()
{
    delete pv;
}
void HltPlayerCtl::setupListView()
{
    playlist = new QListWidget(pv);
    playlist->setIconSize(QSize(thumbWidth, thumbHeight));
    playlist->setMouseTracking(true);
    playlist->setCursor(Qt::PointingHandCursor);
    playlist->viewport()->setAttribute(Qt::WA_Hover);
    playlist->setFlow(QListWidget::LeftToRight);
    connect(playlist,SIGNAL(currentRowChanged(int)),this,SLOT(handleSelectionChanged(int)));
    connect(playlist,SIGNAL(viewportEntered()),pv,SLOT(showControlFrame()));
    playlist->setFocusPolicy(Qt::NoFocus);
}
void HltPlayerCtl::setupPlayer()
{
    pv->initView(NULL);
    pv->enableTimeSlider();
    pv->setRepeat(false);
    pv->showControlFrame();
    pv->showAdvancedControlItems();
    pv->show();
    pv->raise();
    //pv->setFullScreen(true);
    connect(pv,SIGNAL(arrowClicked(bool)),this,SLOT(changeItemByPlayer(bool)));
    connect(pv,SIGNAL(endCurrentMediaItem(bool)),this,SLOT(changeItemByPlayer(bool)));
    connect(pv, SIGNAL(closeButtonClicked()),this,SLOT(closeLibraryWindow()));
}
void HltPlayerCtl::setVideoList2(const QList<highlightData> &clips){

    cd.reserve(clips.size());
    playlist->clear();
    playlist->setIconSize(QSize(thumbWidth, thumbHeight));

    cd = clips;
    QList<highlightData>::iterator it;
    for(it = cd.begin() ; it != cd.end() ; it++){
        addFilteredChild((*it));
    }
    //si se agrego alguno se comienza a reoproducir el primero
    if(playlist->count() > 0)
        playlist->setCurrentRow(0);
    pv->setupTopWidget(playlist);
    //primero se agrega el seleccionado al principio(1)

}
void HltPlayerCtl::addFilteredChild(highlightData &fData){

    QPixmap frame;

    QImage image = QImage(thumbWidth,thumbHeight, QImage::Format_RGB32);
    image.fill(Qt::black);
    frame = QPixmap::fromImage(image);

    addToPlaylist(frame.toImage(),fData);
}
void HltPlayerCtl::addToPlaylist(QImage frame, const highlightData &itemData){

    QString timeStr = QString::number(itemData.realTime.first)+'-'+QString::number(itemData.realTime.second);
    QIcon ico = generateHighlightIcon(QPixmap::fromImage(frame), timeStr,itemData.path.split('/').last());
    QListWidgetItem *hl = new QListWidgetItem(ico, NULL);

    hl->setWhatsThis(itemData.path);
    playlist->addItem(hl);
}
QIcon HltPlayerCtl::generateHighlightIcon(QPixmap image, const QString &textAtt , const QString &pathAtt)
{
    QFont iconFont = QFont("Arial");
    int pixelSize = 9;
    int border = 5;


    int r = hoverColor.red();
    int g = hoverColor.green();
    int b = hoverColor.blue();

    QIcon ico;
    if(image.height() > thumbHeight || image.width() > thumbWidth)
        image = image.scaled(thumbWidth, thumbHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QPainter painter(&image);

    //Draw info label background
    qreal h = ((qreal)image.height())*0.75;
    QRect foregroundRect(border, h,image.width() - (border*2), 20);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QRect rbottom(0, (int)h, image.width(), image.height());
    painter.fillRect(rbottom,QColor(0,0,0,100));

    // Escribe el nombre de la carpeta y el número de elementos
    QPen borderPen;
    painter.setBrush(QBrush(QColor(r,g,b,90),Qt::SolidPattern));
    borderPen.setColor(textColor); // Texto Blanco
    painter.setPen(borderPen);
    iconFont.setPixelSize(pixelSize);
    iconFont.setBold(QFont::Normal);
    painter.setFont(iconFont);
    painter.setRenderHints(QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

    QRect boundingRect;
    painter.drawText(QRect(0,0,image.width(),20), Qt::AlignRight | Qt::AlignVCenter, pathAtt, &boundingRect);

    iconFont.setPixelSize(pixelSize);
    iconFont.setBold(QFont::Normal);
    painter.setFont(iconFont);
    painter.drawText(foregroundRect,Qt::AlignLeft | Qt::AlignVCenter,textAtt);

    ico.addPixmap(image, QIcon::Normal);
    ico.addPixmap(image, QIcon::Active);
    ico.addPixmap(image, QIcon::Selected);

    return ico;
}

void HltPlayerCtl::closeLibraryWindow(){
    pv->stopPlayer();
    pv->close();
}

void HltPlayerCtl::handleSelectionChanged(int index)
{

    if(index != -1){
        highlightData cHigh = cd.at(index);
        //setear speed
        pv->loadSpeed(cHigh.speed);
        pv->setCurrentIndex(cHigh.tagID);
        pv->setTrackPosition(cHigh.realTime.first,cHigh.realTime.second);

        QString fileSelected = cHigh.path;

        if(fileSelected == currentPath)
            pv->seekToEvent();
        else
        {
            if(pv->openMedia(fileSelected)){
                currentPath = fileSelected;

                pv->setPath(fileSelected);
            }
        }
    }

}
void HltPlayerCtl::changeItemByPlayer(bool isFordward,bool isAutorepeat)
{
    if(isAutorepeat)
        playlist->blockSignals(true);
    int nitems = playlist->count();
    if(nitems > 0)
    {
        int crow = playlist->currentRow();
        int sum = 1;
        if(!isFordward)
            sum = -1;

        //Si estoy en el último item y me muevo hacia adelante
        if(crow == nitems-1 && sum == 1){
            if(crow == 0)
                this->handleSelectionChanged(crow);
            else
                playlist->setCurrentRow(0);
            playlist->blockSignals(false);
            return;
        }

        //Si estoy en el primer item y me muevo hacia atras
        if(crow == 0 && sum == -1){
            playlist->setCurrentRow(nitems-1);
           playlist->blockSignals(false);
            return;
        }
        playlist->setCurrentRow(crow + sum);
        playlist->blockSignals(false);

    }
}
