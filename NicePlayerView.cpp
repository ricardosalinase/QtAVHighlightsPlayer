#include <QLayout>
#include <QListWidget>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QMovie>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QMimeData>
#include "pikdinfobox.h"
#include "pikdTimeSlider.h"
#include "pikdplayerview.h"
#include "searchframe.h"
#include "pikddragitemrenderer.h"
#include "sliceframe.h"
#include "pikdxml.h"
#include "CustomPushButton.h"
#include "borderFrame.h"
#include "suggestionlistwidget.h"
#include "utils/customshadoweffect.h"

PikdPlayerView::PikdPlayerView(QWidget *parent) : PlayerView(parent)
  , proxyFordwardButton(0)
  , proxyBackwardButton(0)
  , searchFrame(0)
  , proxyMediaButton(0)
  , proxyShareButton(0)
  , proxyShowListButton(0)
  , proxyEditButton(0)
  , sugListWidget(0)
  , proxySuggestionListWidget(0)
  , photoButton(0)
  , fordwardButton(0)
  , backwardButton(0)
  , dragging(false)
  , editorMode(false)
  , searchMode(false)
  , backFlag(false)
  , editFlag(false)
  , applyFlag(false)
  , m_editorDefByUser(false)
  , m_editorBottomMargin(100)
  , m_editorHeightFactor(0.3)
  , m_editorWidthFactor(0.8)
{
    this->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
    this->hide();
}

void PikdPlayerView::setPromotion(QString prom){
    qDebug()<<"PLAYER ACTIVO PROMOTION"<<prom;
    infoBoxLabel->disconnect();
    connect(infoBoxLabel,SIGNAL(accepted()),this,SIGNAL(doSearch_promotion()));
    infoBoxLabel->showSearchPromotion(prom);
    proxyInfoLabel->show();
}

void PikdPlayerView::setParameters(){
    if(xmlSettings != NULL)
    {
        QDomElement root = xmlSettings->document.firstChildElement();
        QDomElement player = xmlSettings->getClassById(root, "custom_player");

        if(player.isNull())
            qDebug()<< "Is Null";
        {
            xmlSettings->listAllElements(player);

            QString par0 = xmlSettings->getClassTextElement(player,"editorGeometryDefinedByUser");
            if(par0 == "NULL_PARAMETER")
                qDebug() << "pikdWidget::setParameters::Null Value for 'editorGeometryDefinedByUser'' because this does not exist or is repeated, you need to set default parameter";
            else{
                QDomElement editorGeometryElement = xmlSettings->getClassElement(player,"editorGeometryDefinedByUser");
                QString par0_Att = editorGeometryElement.attribute("value");
                if(par0_Att == "true")
                    m_editorDefByUser = true;
                else
                    m_editorDefByUser = false;

                if(m_editorDefByUser)
                {

                    //Editor bottom margin
                    QString par1 = xmlSettings->getClassTextElement(editorGeometryElement,"editorBottomMargin");
                    if(par1 == "NULL_PARAMETER")
                        qDebug() << "pikdWidget::setParameters::Null Value for 'editorBottomMargin'' because this does not exist or is repeated, you need to set default parameter";
                    else{
                        m_editorBottomMargin = par1.toInt();
                    }

                    //Editor Width Factor
                    QString par2 = xmlSettings->getClassTextElement(editorGeometryElement,"editorWidthFactor");
                    if(par2 == "NULL_PARAMETER")
                        qDebug() << "pikdWidget::setParameters::Null Value for 'editorBottomMargin'' because this does not exist or is repeated, you need to set default parameter";
                    else{
                        m_editorWidthFactor = par2.toFloat();
                    }

                    //Editor Height Factor
                    QString par3 = xmlSettings->getClassTextElement(editorGeometryElement,"editorHeightFactor");
                    if(par3 == "NULL_PARAMETER")
                        qDebug() << "pikdWidget::setParameters::Null Value for 'editorHeightFactor'' because this does not exist or is repeated, you need to set default parameter";
                    else{
                        m_editorHeightFactor = par3.toFloat();
                    }

                }else
                {
                    m_editorWidthFactor = 0.826;
                }
            }

        }
    }
}

void PikdPlayerView::toggleEditPlayer(bool active){

    if(!active)
    {
        if(hltCtl != NULL){
            delete hltCtl;
            hltCtl = NULL;
        }
    }

    setUpdatesEnabled(false);

    if(applyButton != NULL){
        applyButton->setVisible(active);
    }
    if(backButton != NULL){
        backButton->setVisible(active);
    }

    if(active){

        //Update Editor Frame Height
        if(!m_editorDefByUser)
        {
            m_editorHeightFactor = (this->sceneRect().width()*m_editorWidthFactor)/12.8911;
            m_editorBottomMargin = (int)((this->sceneRect().height())*0.10);
            frame->setFixedHeight((int)m_editorHeightFactor);
        }else
        {
            frame->setFixedHeight((int)(this->sceneRect().height()*m_editorHeightFactor));
        }

        animation = new QPropertyAnimation(proxyFrame, "geometry");
        //Start Rect
        qreal x0 = 0;
        qreal y0 = this->sceneRect().height() - timeSlider->height();
        qreal w0 = this->visibleRect().width();
        qreal h0 = timeSlider->height();
        QRectF startRect(x0,y0,w0,h0);

        //New Rect Values
        qreal h1 = (proxyFrame->size()).height();
        qreal w1 = (this->sceneRect().width()*m_editorWidthFactor);
        qreal x1 = ( (this->sceneRect().width()) - w1 )/2.0;
        qreal y1 = this->sceneRect().height() - (proxyFrame->size()).height() - ((qreal)m_editorBottomMargin);
        QRect rect(x1,y1,w1,h1);
        animation = new QPropertyAnimation(frame, "geometry");
        animation->setDuration(100);
        frame->setBorder(8);
        qDebug()<<frame->rect();
        animation->setStartValue(startRect);
        animation->setEndValue(rect);
        frame->setFixedWidth(w1);
        proxyFrame->setPos(x1, y1);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
        frame->setStyleSheet("QFrame{"
                             "background-color: rgba(0, 0, 0, 100);"
                             "}"
                             );
    }
    else{
        frame->setBorder(1);
        frame->setFixedHeight(frame->height()/1.95);
        frame->setFixedWidth(this->sceneRect().width());
        frame->setStyleSheet("QFrame{"
                             "background-color: qlineargradient(spread:pad, x1:0 y1:0, x2:0 y2:1, stop:0 rgba(255, 255, 255, 0), stop:1 rgba(0, 0, 0, 100) );"
                             "}"                                          );
        proxyFrame->setPos(0,this->sceneRect().height() - proxyFrame->size().height());
    }

    maxButton->setVisible(!active);
    photoButton->setVisible(!active);
    setEditAndSaveEnabled(!active);
    setUpdatesEnabled(true);

    editorMode = active;
    if(active)
        searchFrame->setVisibleSearchMode(active);
    searchFrame->setupSearch(!active);
}

void PikdPlayerView::setFrameGeometry(){

    frame->setFixedWidth(this->sceneRect().width()*0.90);
    proxyFrame->setPos((this->sceneRect()).width()*0.05, (this->sceneRect()).height() - proxyFrame->size().height());
}

void PikdPlayerView::setEditAndSaveEnabled(bool enabled){
    editButton->setEnabled(enabled);
    mediaButton->setEnabled(enabled);
}

void PikdPlayerView::disableSearchView()
{
    emit toggleSearchMode(false);
}

void PikdPlayerView::toggleStdPlayer(bool active){
    setUpdatesEnabled(false);
    if(searchFrame != NULL){
        proxySearchFrame->setEnabled(active);
    }
    photoButton->setVisible(active);
    setEditAndSaveEnabled(active);
    //frame se rediseña
    if(active){
        showAdvancedControlItems();
    }
    else{
        playlistFrame->removeCenterWidget();
    }
    setUpdatesEnabled(true);
}

PikdPlayerView::~PikdPlayerView()
{
    if(scene != NULL){
        this->deleteProxyWidgets();
    }
}

void PikdPlayerView::deleteProxyWidgets()
{

    if(fordwardButton != NULL){
        freeWidget(fordwardButton);
        delete fordwardButton;
    }

    if(backwardButton != NULL){
        freeWidget(backwardButton);
        delete backwardButton;
    }

    if(mediaButton != NULL){
        freeWidget(mediaButton);
        delete mediaButton;
    }

    if(shareButton != NULL){
        freeWidget(shareButton);
        delete shareButton;
    }

    if(editButton != NULL){
        freeWidget(editButton);
        delete editButton;
    }

    if(sugListWidget != NULL){
        freeWidget(sugListWidget);
        delete sugListWidget;
    }

    if(searchFrame != NULL){
        freeWidget(searchFrame);
        delete searchFrame;
    }
    if(infoBoxLabel != NULL)
    {
        freeWidget(infoBoxLabel);
        delete infoBoxLabel;
    }

    if(gif_anim != NULL)
    {
        freeWidget(gif_anim);
        delete gif_anim;
    }
}

void PikdPlayerView::insertHltCtl()
{
    if(hltCtl != NULL)
    {
        hltCtl->setParent(this);
        showBasicControlItems(); //hide the standard video frame
        //adjust layout for slider
        l->addWidget(hltCtl);
        QVBoxLayout *hltButtonlayout = new QVBoxLayout();
        l->addItem(hltButtonlayout);
        hltButtonlayout->addWidget(applyButton,0,Qt::AlignTop);
        hltButtonlayout->addWidget(backButton,0,Qt::AlignTop);
        //
        //set playerview for edit mode ( hide and show some tools)
        toggleEditPlayer(true);
    }
}

void PikdPlayerView::insertCustomSlider(QWidget *customSlider){
    if(customSlider){
        showBasicControlItems(); //hide the standard video frame
        //adjust layout for slider
        l->addWidget(customSlider);
        QVBoxLayout *hltButtonlayout = new QVBoxLayout();
        l->addItem(hltButtonlayout);
        hltButtonlayout->addWidget(applyButton,0,Qt::AlignTop);
        hltButtonlayout->addWidget(backButton,0,Qt::AlignTop);
        //
        //set playerview for edit mode ( hide and show some tools)
        toggleEditPlayer(true);
    }
}

void PikdPlayerView::initCustomView()
{
    setupUI();
    setupPikdPlayer();
    setupPikdScene();
    //Fullscreen mode
    //this->setFullScreen(false);

    showControlFrame();

    //ajustar frame a diseño de hlt controller
    showAdvancedControlItems();
}

void PikdPlayerView::setupPikdPlayer()
{
    timeSlider->setUseTicks(true);
    connect(timeSlider, SIGNAL(overTick(int)), SIGNAL(tickEntered(int)));
    connect(timeSlider,SIGNAL(tickClicked(int)),this,SLOT(tickClickedSlot(int)));
    connect(photoButton,SIGNAL(clicked()),this,SLOT(takePhotoSlot()));
    connect(backwardButton,SIGNAL(clicked()),this,SLOT(backwardButtonClickedSlot()));
    connect(fordwardButton,SIGNAL(clicked()),this,SLOT(fordwardButtonClickedSlot()));
    connect(shareButton,SIGNAL(clicked()),this,SLOT(shareButtonClickedSlot()));
    connect(editButton,SIGNAL(clicked()),this,SLOT(editButtonClickedSlot()));
    connect(mediaButton,SIGNAL(clicked()),this,SLOT(mediaButtonClickedSlot()));
    connect(backButton,SIGNAL(clicked()),this,SLOT(backButtonClickedSlot()));
    connect(applyButton,SIGNAL(clicked()),this,SLOT(applyButtonClickedSlot()));
    connect(searchFrame, SIGNAL(searchButtonClicked(QString)),this,SLOT(doSearch_dbInterSlot(QString)));
    //connect(searchFrame,SIGNAL(backNoSearch()),this,SIGNAL(backNoSearch()));
    connect(this,SIGNAL(toggleSearchMode(bool)),searchFrame,SLOT(setVisibleSearchMode(bool)));
    //maxButton->hide();
    //disconnect(maxButton,SIGNAL(clicked(bool )),this,SLOT(setFullScreen(bool)));
    disconnect(videoItem, SIGNAL(videoRendererClicked()),this, SLOT(videoRendererClickedSlot()));
}

void PikdPlayerView::shareButtonClickedSlot()
{
    QRectF shareRect(proxyShareButton->pos(),proxyShareButton->size());
    proxygif_anim->setGeometry(shareRect);
    proxyShareButton->hide();
    gif_anim->movie()->start();
    proxygif_anim->show();
    emit shareButtonClicked();
}

void PikdPlayerView::doSearch_dbInterSlot(const QString &str)
{
//    setSearchMode(true);
    emit doSearch_dbInter(str);
}

bool PikdPlayerView::getSearchMode() const
{
    return searchMode;
}

void PikdPlayerView::setSearchMode(bool value)
{
    if(editorMode && searchMode){

        emit backButtonClicked();
    }
    searchMode = value;
    toggleCloseButtonStyleSheet(searchMode);
}


void PikdPlayerView::clearSuggestionListData()
{
    if(sugListWidget != NULL)
        sugListWidget->clear();
}

void PikdPlayerView::initSuggestionList(const QStringList &sList){

//    if(sugListWidget != NULL)
//    {
//        freeWidget(sugListWidget);
//        delete sugListWidget;
//        sugListWidget = NULL;
//    }

    sugListWidget->resetSuggestionList(sList);
    CustomShadowEffect *bodyShadow = new CustomShadowEffect();
    bodyShadow->setBlurRadius(20.0);
    bodyShadow->setDistance(6.0);
    bodyShadow->setColor(QColor(0, 0, 0, 150));
    proxySuggestionListWidget->setGraphicsEffect(bodyShadow);
    proxySuggestionListWidget->setFocusPolicy(Qt::ClickFocus);
    proxySuggestionListWidget->hide();
    proxySuggestionListWidget->setZValue(2.5);

    connect(searchFrame,&SearchFrame::showSuggestion,[=](bool visible){
        emit(blockTimer());
        if(mediaMode) return;
        if(visible){
        proxySearchFrame->setZValue(3.0);
        sugListWidget->setMinimumWidth( searchFrame->getWidthWithOutSearchButton() );
        sugListWidget->adjustHeightToContents();
        proxySuggestionListWidget->setPos(searchFrame->x(),searchFrame->y() - (int)((qreal)sugListWidget->height()*0.95) );
        proxySuggestionListWidget->show();
        }
        else{
            proxySuggestionListWidget->hide();
        }
        qDebug()<<"PROXY IS VISIBLE?"<<proxySuggestionListWidget->isVisible();
        qDebug()<<"LIST IS VISIBLE?"<<sugListWidget->isVisible();
    });

    connect(sugListWidget, &QListWidget::itemPressed,this,&PikdPlayerView::setAndSendText);
}

void PikdPlayerView::setAndSendText(QListWidgetItem *item){
    searchFrame->lineEdit->setText(item->text());
    emit doSearch_dbInter(item->text());
}

void PikdPlayerView::setMediaMode(bool value)
{
    mediaMode = value;
}
void PikdPlayerView::mousePressEvent(QMouseEvent *event){
    PlayerView::mousePressEvent(event);
    if(proxySuggestionListWidget != NULL){
        proxySuggestionListWidget->hide();
        if(proxySearchFrame != NULL)
            proxySearchFrame->setZValue(2.0);
    }
}

void PikdPlayerView::tickClickedSlot(int idx)
{
    if(!timeSlider->highlights.isEmpty() && timeSlider->highlights.size() > 0){
        QVector<tick>::iterator it;
        for(it = timeSlider->highlights.begin(); it != timeSlider->highlights.end(); ++it)
        {
            if((*it).id == idx){
                if(editorMode)
                    emit tickClickedSignal();
                else
                    player->seek( (qint64)(*it).lpos );
            }
        }

        selectTick(idx);
    }
}
void PikdPlayerView::backwardButtonClickedSlot()
{
    qDebug()<<"pikdPlayerView::Backward Button Clicked";
    if(editorMode)
        emit backButtonClickedSlot();
    emit arrowClicked(false,false);
}

void PikdPlayerView::fordwardButtonClickedSlot()
{
    qDebug()<<"pikdPlayerView::Fordward Button Clicked";
    if(editorMode)
        emit backButtonClickedSlot();
    emit arrowClicked(true,false);
}
void PikdPlayerView::editButtonClickedSlot()
{
    qDebug()<<"pikdPlayerView::Edit Button Clicked";
    editFlag = true;
}

void PikdPlayerView::mediaButtonClickedSlot()
{
    qDebug()<<"pikdPlayerView::Media Button Clicked";
    if(!videoItem->getMimeDataText().isEmpty()){
        QRectF mediaRect(proxyMediaButton->pos(),proxyMediaButton->size());
        proxygif_anim->setGeometry(mediaRect);
        proxyMediaButton->hide();
        gif_anim->movie()->start();
        proxygif_anim->show();
        emit mediaButtonDropped( videoItem->getMimeDataText() );
    }
}

void PikdPlayerView::backButtonClickedSlot()
{
    qDebug()<<"pikdPlayerView::Back Button Clicked";
    backFlag = true;
}

void PikdPlayerView::applyButtonClickedSlot(){
    applyFlag = true;
}
void PikdPlayerView::initProxyButton(QPushButton **btn,QGraphicsProxyWidget **proxy,QSize size, const QString &style, QWidget *parent, bool checkeable)
{
    (*btn) = new QPushButton(parent);
    (*btn)->setCursor(Qt::PointingHandCursor);
    (*btn)->setFlat(true);
    if(checkeable)
        (*btn)->setCheckable(checkeable);
    (*btn)->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
    (*btn)->setFixedSize(size);
    (*btn)->setStyleSheet(style);

    (*proxy) = scene->addWidget(*btn);
    CustomShadowEffect *bodyShadow = new CustomShadowEffect();
    bodyShadow->setBlurRadius(20.0);
    bodyShadow->setDistance(6.0);
    bodyShadow->setColor(QColor(0, 0, 0, 150));
    (*proxy)->setGraphicsEffect(bodyShadow);
}
void PikdPlayerView::mouseMoveEvent(QMouseEvent *e)
{
    if(closeButtonEnabled)
        setArrowVisible(true);
    //showAllControlItems();

    if(!proxyInfoLabel->isVisible())
    QGraphicsView::mouseMoveEvent(e);
}

void PikdPlayerView::showAllControlItems(){
//    if(proxyFrame->isVisible())
//        return;

    showControlFrame();
    playButton->show();
    volumeButton->show();
    timeSlider->show();
    closeButton->show();
    photoButton->show();

}

void PikdPlayerView::setArrowVisible(bool isVisible)
{
    if(isVisible)
    {
        if(fordwardButton != NULL && !fordwardButton->isVisible())
            fordwardButton->show();

        if(backwardButton != NULL && !backwardButton->isVisible())
            backwardButton->show();
    }
    else
    {
        if(fordwardButton != NULL && fordwardButton->isVisible())
            fordwardButton->hide();


        if(backwardButton != NULL && backwardButton->isVisible())
            backwardButton->hide();
    }
}

void PikdPlayerView::resizeEvent(QResizeEvent *event){

    setUpdatesEnabled(false);
    QSizeF viewSize = (this->visibleRect()).size();
    int margin = viewSize.width()*0.01;

    if(proxyFordwardButton != NULL){


        //    proxyMediaButton->setPos(margin,
        //                             viewSize.height() - (proxyMediaButton->size()).height() - (proxyFrame->size()).height() - margin);

        proxyFordwardButton->setPos(viewSize.width() - (proxyFordwardButton->size()).width() - margin, (viewSize.height() - (proxyFordwardButton->size()).height())/2.0);
        proxyBackwardButton->setPos(margin, (viewSize.height() - (proxyBackwardButton->size()).height())/2.0);

        proxySearchFrame->setPos(margin,
                                 viewSize.height() - (proxySearchFrame->size()).height() - (timeSlider->size()).height() - margin);

        proxyMediaButton->setPos(margin,
                                 viewSize.height() - (proxyMediaButton->size()).height() - (timeSlider->size()).height() - (margin*2) - (proxySearchFrame->size()).height());

        proxyEditButton->setPos(viewSize.width() - (proxyEditButton->size()).width() - margin,
                                viewSize.height() - (proxyEditButton->size()).height() - (timeSlider->size()).height() - (margin*2) - (proxySearchFrame->size()).height());

        proxyShareButton->setPos(viewSize.width() - (proxyShareButton->size()).width() - margin,
                                  proxySearchFrame->y() + searchFrame->layout()->margin());

        proxySuggestionListWidget->setPos(searchFrame->x(),searchFrame->y() - (int)((qreal)sugListWidget->height()*0.95) );

        if(!proxyMediaButton->isVisible())
        {
            proxygif_anim->setPos(proxyMediaButton->pos());
            int size = ((elliShare->rect().width() - proxyShareButton->size().width()) / 2);
            int xC = proxyMediaButton->x() - size;
            int yC = proxyMediaButton->y() - size;
            elliMedia->setPos(xC,yC);
        }
        else if(!proxyShareButton->isVisible())
        {
            proxygif_anim->setPos(proxyShareButton->pos());
            int size = ((elliShare->rect().width() - proxyShareButton->size().width()) / 2);
            int xC = proxyShareButton->x() - size;
            int yC = proxyShareButton->y() - size;
            elliShare->setPos(xC,yC);
        }
    }

    changeCloseButtonPos();
    qDebug()<<"pikdPlayerView::-----Resize Event-----";
    videoItem->resizeRenderer(viewSize.width(), viewSize.height());
    scene->setSceneRect(0,0,viewSize.width(),viewSize.height());
    if(!editorMode)
        proxyFrame->setGeometry(QRect(proxyFrame->x(),viewSize.height() - (proxyFrame->size()).height(),
                            viewSize.width(), (proxyFrame->size()).height()));
    else{
        proxyFrame->setGeometry(QRect( viewSize.width()*0.10, proxyFrame->y(),
                            viewSize.width()*0.90, (proxyFrame->size()).height()));

    }

    proxyPlaylist->setGeometry(QRect(0,0,viewSize.width(), 110));

    qDebug()<<"Tamaño del Graphics View: "<<viewSize.width()<<" x "<<viewSize.height();
    qDebug()<<"Tamaño de la Escena: "<<scene->width()<<" x "<<scene->height();
    QGraphicsView::resizeEvent(event);
    setUpdatesEnabled(true);

}

void PikdPlayerView::keyPressEvent(QKeyEvent *e)
{
    qDebug()<<"KEY PRESS EVENT FROM PLAYER --> FOCO"<<this->focusWidget()<<searchFrame->focusWidget();

    if(searchFrame->lupeButton->isChecked() && e->key() == Qt::Key_Return){
        this->doSearch_dbInterSlot( searchFrame->getLineEditValue() );
    }
    else if(e->key() == Qt::Key_Escape){
        //ESCAPE BUTTON POLICY
        //EDITOR MODE - SEARCH MODE - FULLSCREEN MODE - CLOSE PLAYER
        if(editorMode)
            emit backButtonClicked();
        else if (searchMode)
            emit closeButtonClicked();
        else if(this->parentWidget()->parentWidget()->parentWidget()->isFullScreen())
            emit fullscreen();
        else
            emit closeButtonClicked();
    }
    else if (!searchFrame->lupeButton->isChecked()){
        switch(e->key()){
        case Qt::Key_Space:{
            qDebug()<<"EL FOCO ESTA EN"<<this->focusWidget()->objectName();
            emit play();
        }
        break;
        case Qt::Key_Left:
            if(e->isAutoRepeat() && getPlayerState() != QtAV::AVPlayer::PausedState )
                emit arrowClicked(false,true);
        break;
        case Qt::Key_Right:
            if(e->isAutoRepeat() && getPlayerState() != QtAV::AVPlayer::PausedState )
                emit arrowClicked(true,true);
        break;
        }
    }

    PlayerView::keyPressEvent(e);
}

void PikdPlayerView::keyReleaseEvent(QKeyEvent *e){
    qDebug()<<"KEY RELEASE EVENT FROM PLAYER --> "<<e->key()<<"AUTOREPEAT"<<e->isAutoRepeat();

    if(e->isAutoRepeat()){
        e->ignore();
    }
    else if (!searchFrame->lupeButton->isChecked()){
        switch(e->key()){
        case Qt::Key_Left:{
            if(getPlayerState() == QtAV::AVPlayer::PlayingState ){
//                emit listKeyReleased();
                emit arrowClicked(false,false);
            }
        }
            break;
        case Qt::Key_Right:{
            if(getPlayerState() == QtAV::AVPlayer::PlayingState ){
                emit arrowClicked(true,false);
//                 emit listKeyReleased();
            }
        }
            break;
        }
    }
   PlayerView::keyReleaseEvent(e);
}
void PikdPlayerView::mouseReleaseEvent(QMouseEvent *e)
{
    QGraphicsView::mouseReleaseEvent(e);
    if(backFlag)
    {
        //setFullScreen(false);
        emit backButtonClicked();
        proxySuggestionListWidget->hide();
        backFlag = false;
        return;
    }
    if(applyFlag)
    {
        //setFullScreen(false);
        emit applyButtonClicked();
        proxySuggestionListWidget->hide();

        applyFlag = false;
        return;
    }

    if(editFlag){
        //setFullScreen(false);
        emit editButtonClicked();
        proxySuggestionListWidget->hide();
        editFlag = false;
        return;
    }
    PlayerView::mouseReleaseEvent(e);
}

void PikdPlayerView::setupPikdScene(){

    backButton->hide();
    applyButton->hide();

    proxySearchFrame = scene->addWidget(searchFrame);

    proxyMediaButton->setAcceptDrops(true);
    elliShare = scene->addEllipse(0,0,shareButton->width()*1.2,shareButton->height()*1.2,QPen(Qt::transparent),QBrush(QColor(0,0,0,100)));
    elliMedia = scene->addEllipse(0,0,mediaButton->width()*1.2,mediaButton->height()*1.2,QPen(Qt::transparent),QBrush(QColor(0,0,0,100)));
    elliShare->hide();
    elliMedia->hide();
    proxyMediaButton->setZValue(2.0);
    proxyShareButton->setZValue(2.0);
    proxyEditButton->setZValue(2.0);
    proxySearchFrame->setZValue(2.0);
    proxygif_anim = scene->addWidget(gif_anim);
    proxygif_anim->hide();

    proxyInfoLabel = scene->addWidget(infoBoxLabel,Qt::Widget);
    proxyInfoLabel->hide();

    proxySuggestionListWidget = scene->addWidget(sugListWidget);
    proxySuggestionListWidget->hide();
    disableTimeSlider();
}

void PikdPlayerView::setupUI(){
    //    //Generar los items necesarios para manejar la escena de video
    //    //Create playlist Frame Container

//    //Create frame
    searchFrame = new SearchFrame();
    searchFrame->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
    searchFrame->setFixedSize(QSize(600,71));
    searchFrame->setFocusPolicy(Qt::StrongFocus);
    QSize size1(51,51); //Close, Fordward, Backward Buttons
    QSize size2(51,51); //Edit and Media Buttons
    QSize size3(23,23); //play, volume, rate, photo, max, edit buttons
    QSize size4(92,23); //apply cancel button
    QSize size5(51,61); //Share Button

    this->initProxyButton(&shareButton, &proxyShareButton, size5,
                     "QPushButton {"
                      "background-color: transparent;"
                      "border-image: url(:/resources/share-activo.png)}");

    this->initProxyButton(&mediaButton,&proxyMediaButton, size2,
                     "QPushButton {"
                      "background-color: transparent;"
                      "border-image: url(:/resources/save-activo.png)}"
                      "QPushButton:hover { border-image: url(:/resources/save-activo.png)"
                     "}");
    //Video edition
    this->initProxyButton(&editButton,&proxyEditButton, size2, "QPushButton {"
                                        "background-color: transparent;"
                                        "border-image: url(:/hltTools/resource/pikdplayer/edit.png)}"
                                        "");

//    //Backward and Fordward Buttons
    this->initProxyButton(&backwardButton,&proxyBackwardButton, size1, "QPushButton{"
                                                                     "background-color: transparent;"
                                                                     "border-image: url(:/resources/back.png);}"
                                                                     "QPushButton:hover{"
                                                                     "border-image: url(:/resources/back-hover.png);}" );

    this->initProxyButton(&fordwardButton,&proxyFordwardButton, size1, "QPushButton{"
                                                                     "background-color: transparent;"
                                                                     "border-image: url(:/resources/next.png);}"
                                                                     "QPushButton:hover{"
                                                                     "border-image: url(:/resources/next-hover.png);}" );

    this->initButton(&photoButton, size3, "QPushButton{"
                                                      "background-color: transparent;"
                                                      "border-image: url(:/resources/camara.png)}"
                                                      "QPushButton:hover { border-image: url(:/resources/camara-activa.png)}"
                                                      "QPushButton:disabled {border-image: url(:/resources/camara-inhabilitado.png)}", frame);

    this->initButton(&backButton,size4,"QPushButton {"
                                       "background-color: black;"
                                       "color: white;"
                                       "border: 2px solid black;"
                                       "border-radius: 4px;"
                                       "}",frame);
    backButton->setText("Cancel");
    this->initButton(&applyButton,size4,"QPushButton {"
                                        "background-color: black;"
                                        "color: white;"
                                        "border: 2px solid black;"
                                        "border-radius: 4px;"
                                        "}",frame);
    applyButton->setText("Apply");

    l->addWidget(photoButton);
    l->addWidget(maxButton);

//    if(!editorMode)

//    else
//        editButton->hide();

    //Gif para animación del progreso
    gif_anim = new QLabel();
    gif_anim->setStyleSheet("QLabel{ background-color: transparent;}");
    gif_anim->resize(size2);
    gif_anim->setMovie(new QMovie(":/resources/progress.gif"));

    //Label de información
    infoBoxLabel = new PikdInfobox();
    infoBoxLabel->setWindowOpacity(0.8);
    infoBoxLabel->setStyleSheet("QDialog{background-color: rgb(0,0,0,100);}"
                                "QLabel{background-color: transparent;"
                                "color: white;}");

//    infoBoxLabel->hide();
//    infoBoxLabel->setStyleSheet("QLabel{"
//                                    "font: 25 10pt 'Frutiger CE 45 Light';"
//                                    "font-weight: bold;"
//                                    "background-color: rgba(26,26,26,100);"
//                                    "min-width: 500px;"
//                                    "min-height: 300px;"
//                                    "font-size: 24px;"
//                                    "color: rgb(255,255,255);"
//                                    "qproperty-alignment: AlignCenter;"
//                                "}");


    sugListWidget = new SuggestionListWidget();
    sugListWidget->setBorder(4);
}

void PikdPlayerView::stopProgressBar(){
    gif_anim->movie()->stop();

    if(!proxyMediaButton->isVisible()){
        gif_anim->hide();
        proxyMediaButton->show();
        elliMedia->hide();
    }
    else if(!proxyShareButton->isVisible()){
        gif_anim->hide();
        proxyShareButton->show();
        elliShare->hide();
    }
}

void PikdPlayerView::dragEnterEvent(QDragEnterEvent *e){
    qDebug()<<"PikdPlayerView::Drag Enter Event";
    e->acceptProposedAction();
    int size = ((elliShare->rect().width() - proxyShareButton->size().width()) / 2);

    QSizeF viewSize = this->visibleRect().size();
    int xC = proxyShareButton->x() - size;
    int yC = proxyShareButton->y() - size;
    elliShare->setPos(xC,yC);
    xC = proxyMediaButton->x() - size;
    yC = proxyMediaButton->y() - size;
    elliMedia->setPos(xC,yC);
    elliMedia->show();
    elliShare->show();
    infoBoxLabel->setIcon("","");
    infoBoxLabel->setText("Drop onto icon to save or share","");
    infoBoxLabel->setButtons("","");
    proxyInfoLabel->setPos((viewSize.width()/2) - ((proxyInfoLabel->size()).width()/2), (viewSize.height()/2) - ((proxyInfoLabel->size()).height()/2));
    proxyInfoLabel->show();
    infoBoxLabel->show();
    //emit messageToBox("Drop onto icon to save or share");

    PlayerView::dragEnterEvent(e);
}
void PikdPlayerView::dragMoveEvent(QDragMoveEvent *e){

    QPointF scenePos = this->mapToScene(e->pos());
    if(!dragging)
        dragging = true;

    QRectF mediaRect(proxyMediaButton->pos(),proxyMediaButton->size());
    QRectF shareRect(proxyShareButton->pos(),proxyShareButton->size());
    //Si se encuentra en media o share acepta el evento
    if(mediaRect.contains(scenePos) || shareRect.contains(scenePos)){
        e->acceptProposedAction();
    }
    else{
        e->ignore();
    }
}
void PikdPlayerView::dropEvent(QDropEvent *e){

    QPointF scenePos = this->mapToScene(e->pos());
    QRectF mediaRect(proxyMediaButton->pos(),proxyMediaButton->size());
    QRectF shareRect(proxyShareButton->pos(),proxyShareButton->size());

    if(mediaRect.contains(scenePos)){
        proxygif_anim->setGeometry(mediaRect);
        proxyMediaButton->hide();
        gif_anim->movie()->start();
        proxygif_anim->show();
        qDebug()<<"Media Button dropped"<<e->mimeData()->text();
        emit mediaButtonDropped(e->mimeData()->text());
    }
    else if(shareRect.contains(scenePos)){
        proxygif_anim->setGeometry(shareRect);
        proxyShareButton->hide();
        gif_anim->movie()->start();
        proxygif_anim->show();
        emit shareButtonDropped(e->mimeData()->text());
    }

    hideDragWidgets();
    dragging = false;
}

void PikdPlayerView::dragLeaveEvent(QDragLeaveEvent *e){

    qDebug()<<"PikdPlayerView::Drag Leave Event";
    hideDragWidgets();
    PlayerView::dragLeaveEvent(e);
}
void PikdPlayerView::hideDragWidgets()
{
    infoBoxLabel->hide();
    proxyInfoLabel->hide();
    elliMedia->hide();
    elliShare->hide();
}

void PikdPlayerView::blockPlaylistChange(bool isBlock){

    playlistFrame->disableArrows(isBlock);
    backwardButton->setDisabled(isBlock);
    fordwardButton->setDisabled(isBlock);
}

void PikdPlayerView::temporalSeek(qint64 newTime){
    if(newTime>=0 && newTime<= player->stopPosition()){
        if(newTime < player->startPosition())
            player->setStartPosition(newTime);
            player->seek(newTime);
    }
}
void PikdPlayerView::seekAndPause(bool flag){
    //managePlayStatus();
    //primer aproach
    qDebug()<<"pikdPlayerView::pause emited from seekAndPause()";
    this->player->pause(flag);
}

void PikdPlayerView::seekAndPlay(qint64 pos)
{
    qDebug() << "pikdPlayerView:: Seek and Play";
    qDebug()<<"pikdPlayerView::pause emited from seekAndPlay()";
    player->setPosition(pos);
    player->pause(false);

}
void PikdPlayerView::pauseForcedSlot()
{
    qDebug()<<"pikdPlayerView::pause emited from pauseForcedSlot()";
    player->pause(true);
}
void PikdPlayerView::playFromPauseForcedSlot()
{
    qDebug()<<"pikdPlayerView::pause emited from pauseForcedSlot()";
    if(player->isPaused())
        player->pause(false);
}

void PikdPlayerView::managePlayStatus()
{
    qDebug()<< "Player::manage play status :: state"<<playerState;
    switch (playerState) {
    case QtAV::AVPlayer::StoppedState:
        emit playForced();
    case QtAV::AVPlayer::PausedState:
        emit playForced();
        break;
    default:
        qDebug()<< "Player::Play/Pause_Warning: No State for Player";
    }
}

void PikdPlayerView::setLimitsEditor(){
    sliderLimitsChangeSlot(0,player->duration());
    emit videoLoaded();
}

void PikdPlayerView::statusChanged(QtAV::MediaStatus status)
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
        if(editorMode){
            sliderLimitsChangeSlot(0,player->duration());
        }
        else if(player->duration() != 0){
            sliderLimitsChangeSlot(startTrackPos,endTrackPos);
            this->updateTicksOnViewFromMap(currentTicks);
        }
        //videoItem->setSpeed(this->playbackRate());
        timeSlider->setSrcDuration(player->duration());
        emit videoLoaded();
    }
        break;
    case QtAV::BufferingMedia:{
        currentPosLabel->setText("Loading...");
        qDebug()<<"Buffering Media";
    }
        break;
    case QtAV::BufferedMedia:{
        qDebug()<<"Buffered Media";
    }
        break;
    case QtAV::LoadingMedia:
        qDebug()<<"Loading Media";
        break;
    case QtAV::StalledMedia:
        qDebug()<<"Media Stalled";
        break;
    case QtAV::EndOfMedia:
    {
        qDebug()<<"End Of Media";
        if(this->player->file() == this->path)
            emit endCurrentMediaItem(true);
    }
        break;
    case QtAV::InvalidMedia:
        displayErrorMessage(QtAV::AVError::FormatError);
        break;
    }

//    qDebug()<<"-----------------------------------------------";
}

void PikdPlayerView::takePhotoSlot()
{
    if(player != NULL)
    {
        qDebug()<<"Take Photo at "<<player->position()/1000<<" [sec]";
        emit photoButtonClicked(player->position());
    }else
        qDebug()<<"Player::player object is NULL";
}
void PikdPlayerView::seekToEvent(){
//      qDebug()<<"SEEKTOEVENT";
      sliderLimitsChangeSlot(startTrackPos,endTrackPos);
      player->seek(startTrackPos);
//      connect(this,SIGNAL(initPlayer(qint64)),player,SLOT(seek(qint64)));
      this->updateTicksOnViewFromMap(currentTicks);
      emit playForced();
}
void PikdPlayerView::updateTicksOnViewFromMap(const QMultiMap<QString, hltPar> &times)
{
    qDebug()<< "Childs Size: "<< times.size();
    timeSlider->deleteAllTicks();
    if(!times.isEmpty())
    {
        QMultiMap<QString, hltPar>::const_iterator it = times.begin();
        int idx = 0;
        while(it != times.end())
        {
            hltPar ctimes = it.value();
            qint32 timeTick = (ctimes.first + ctimes.second)/2;
            if(checkTickInTimeLine(timeTick)){
                tick t;
                t.id = idx;
                t.pos = timeTick;
                t.lpos = ctimes.first;
                t.rpos = ctimes.second;
                t.isRect = true;
                t.typeName = it.key();
                //t.typeName = "highlight";
                timeSlider->insertTick(t, timeSlider->highlights);
//                qDebug() << "Tick "<< idx << " added"<<t.lpos<<t.rpos<<t.typeName;
            }
            idx++;
            ++it;
        }
    }else
        qDebug()<<"pikdPlayerView::Highlights List is Null, We were unable to insert the time slider";
}


void PikdPlayerView::sliderLimitsChangeSlot(qint64 init, qint64 end)
{
//    qDebug()<<"sliderLimitsChange"<<"init"<<init<<"end"<<end<<"duration"<<player->duration();
    if(init >= 0 && /*end <= player->duration() &&*/ init < end){
//        qDebug()<<"pikdPlayerView::view zone changed to: "<< init<< " - "<< end;
        if(setSliderLimitTimes(init,end)){
            timeSlider->setRange(0, int((endTime-startTime)/m_unit));
            timeSlider->setExtUnit(m_unit);
            timeSlider->setExtInitValue(startTime);
            player->setStartPosition(startTime);

//            qDebug()<<"Duration Changed Editor Mode";
            this->updateVideoItemTimes(hltPar((qint32)startTime,(qint32)endTime));
                if(editorMode)
                    emit durationChanged(startTime, endTime);


        }else
            qDebug()<<"sliderLimitsChange::setSliderLimitTimes error! ";
    }
}
void PikdPlayerView::updateVideoItemTimes(hltPar time)
{
    this->videoItem->setTimes(time.first, time.second);
//    player->setSeekType(QtAV::AccurateSeek);
}
void PikdPlayerView::setEditorMode()
{
    emit(blockTimer());
    editorMode = true;
    searchFrame->setVisibleSearchMode(false);
    searchFrame->setupSearch(false);
}
void PikdPlayerView::updateSlot(hltPar time, bool seekToInit){

    qDebug()<< "pikdPlayerView::UpdateSlot";
    //qDebug()<< "Index: "<< index;
    //updateCurrentTickTree(time,index); //Player
    if(seekToInit)
        player->seek((qint64)time.first); //Seek
    else
        player->seek((qint64)time.second);
}

void PikdPlayerView::setROItimes(hltPar times)
{
    startTimeROI = times.first;
    endTimeROI = times.second;
}

void PikdPlayerView::updateCurrentTickTree(const hltPar &time, int idx){

    //Check if timeSlider have ticks
    if(timeSlider->highlights.size() <= 0 || timeSlider->highlights.size() <= idx)
        return;

    qint32 timeTick = (time.first + time.second)/2;
    //Check if time is in timeline
    if(checkTickInTimeLine(timeTick)){
        tick t;
        t.id = idx;
        t.pos = timeTick;
        t.lpos = time.first;
        t.rpos = time.second;
        t.isRect = true;
        t.typeName = timeSlider->highlights[idx].typeName;
        timeSlider->updateTick(t,idx, timeSlider->highlights);
    }
}

bool PikdPlayerView::checkTickInTimeLine(qreal pos)
{
    if(startTime <= pos && pos <= endTime)
        return true;
    else return false;
}
void PikdPlayerView::setCurrentTicks(const QMultiMap<QString, hltPar> &value)
{
    currentTicks = value;
}

void PikdPlayerView::initHltCtlUI()
{
    hltCtl = new HltCtlView;
//    hltCtl->setObjectName(QStringLiteral("hltCtl"));
    hltCtl->setEnabled(false);
    QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(hltCtl->sizePolicy().hasHeightForWidth());
    hltCtl->setSizePolicy(sizePolicy2);
    hltCtl->setMinimumSize(QSize(100, 50));
    hltCtl->setMaximumSize(QSize(16777215, 16777215));

    hltCtl->setFocusPolicy(Qt::NoFocus);
    //hltCtl->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
    hltCtl->setStyleSheet(QStringLiteral("background-color: rgba(255,255,255,0);"
                                         "border: 0px solid transparent;"));
    hltCtl->setFrameShape(QFrame::NoFrame);
    hltCtl->setFrameShadow(QFrame::Raised);

    connect(hltCtl, SIGNAL( sliderModified(hltPar) ), this, SLOT(setROItimes( hltPar )) );
    connect(hltCtl,SIGNAL(leave()),this,SLOT(onTimeSliderLeave()));
    connect(hltCtl,SIGNAL(changeTickPos(qint64)), this->player, SLOT(seek(qint64)));
    connect(hltCtl,SIGNAL(selectTickFinished(qint64)), this, SLOT(seekAndPlay(qint64)));
    connect(this,SIGNAL(positionChanged_hltView(qint64)),hltCtl,SLOT(updateTick(qint64)));
    connect(this, SIGNAL(durationChanged(qint64, qint64 )),hltCtl, SLOT(durationChanged(qint64, qint64 )));
    connect(hltCtl,SIGNAL(pauseSignal_player(bool)),this,SLOT(seekAndPause(bool)));
    connect(hltCtl,SIGNAL(onTimeLine(int)),this,SLOT(showToolTipExt(int)));
    connect(hltCtl,SIGNAL(sliderSelectedFinished(hltPar)),this,SLOT(updateVideoItemTimes(hltPar)));
    connect(hltCtl,SIGNAL(seekTemporalEdiToPlayer(qint64)),this,SLOT(temporalSeek(qint64)));
}

void PikdPlayerView::enableButtons()
{
    this->enableTimeSlider();

    hltCtl->setEnabled(true);
    hltCtl->setMouseTracking(true);

    this->setEnabled(true);
    this->setMouseTracking(true);
}

