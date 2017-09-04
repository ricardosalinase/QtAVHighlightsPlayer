#include "HltCtlView.h"
#include <QDebug>
#include <QFontDatabase>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QGraphicsEffect>
#include <QApplication>
#include <QMimeData>
#include <QDrag>
#include "hltviewutils.h"

HltCtlView::HltCtlView(QWidget *parent):
    QGraphicsView(parent)
  , tickItem(0)
  , zoomedTimeLine(0)
  , scene(0)
  , xmlSettings(0)
  , initZoomedTimeLine(-1)
  , endZoomedTimeLine(-1)
  , prevInitZoomedTimeLine(-1)
  , prevEndZoomedTimeLine(-1)
  , initPlayerTimePos(-1)
  , endPlayerTimePos(-1)
  , firstTick(true)
  , currentTickPos(-1)
  , tickOnInit(false)
  , splitState(false)
  , mergeState(false)
  , useThumbList(true)
  , m_thumbNum(10)
  , m_sliderHeightFactor(0.7)
  , m_zoomFactor(0.333)
  , m_trickIconSize(-1,-1)
  , updateTickFlag(false)
  , pixelPosition(0)
  , editingSlider(false)
  , minimalZoom(1000)
  , m_sliderFontSize(6)
  , path("NULL")
  , myHltSlider(0)
{

}

HltCtlView::~HltCtlView()
{

//    deleteSliders();
//    deleteCircles();

    if(zoomedTimeLine!=NULL)
        delete zoomedTimeLine;

    if(tickItem != NULL)
        delete tickItem;

    if(myHltSlider != NULL)
        delete myHltSlider;

    if(scene != NULL)
        delete scene;
}

void HltCtlView::setHltCtlSize(int neighWidgetHeight, qreal ratioToNeighWidget)
{
    //Set Fixed Height
    qreal playerHltCtlSize = (qreal)(this->height() + neighWidgetHeight /*+ this->layout()->spacing()*/);
    qreal newHltCtlSize = ratioToNeighWidget*playerHltCtlSize;
    this->setFixedHeight((int)newHltCtlSize);
}

void HltCtlView::initHltController(niceXml *reader)
{
    xmlSettings = reader;

    this->setParameters();
    this->setSceneView();
}


void HltCtlView::updateSceneGeometry()
{
    this->setGeometry(0,0,this->width(),this->height());
}

void HltCtlView::setOneThumbnail(const QPixmap &pix, int idx)
{
    qDebug()<<"HltCtlView::Set One Thumbnail"<<thumbWidth<<thumbHeight;
    qreal xPos = ((qreal)idx)*((endPosZoomedTimeLine.x()-initPosZoomedTimeLine.x())/((qreal)m_thumbNum));
    qreal yPos = initPosZoomedTimeLine.y() - (thumbHeight/2.0)/*-(sliderHeight*0.8)/2.0*/;
    QPointF pos(xPos,yPos);

    thumbItems[idx]->setPos(pos);
    thumbItems[idx]->setPixmap(pix.scaled((int)ceil(thumbWidth),(int)ceil(thumbHeight),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    thumbItems[idx]->setTransformationMode(Qt::SmoothTransformation);
    //this->update();
}

bool HltCtlView::getUseThumbList() const
{
    return useThumbList;
}

bool HltCtlView::equalZoomedLimits()
{
    //    qDebug()<<"Check Equal Zommed Limits: \n"<<
//              "InitPrev: "<<prevInitZoomedTimeLine<<" EndPrev: "<<prevEndZoomedTimeLine<<
//              "InitNew: "<<initZoomedTimeLine<<" NewEnd: "<<endZoomedTimeLine;

    bool ret = (initZoomedTimeLine == prevInitZoomedTimeLine
            && endZoomedTimeLine == prevEndZoomedTimeLine)?
                true : false;
    return ret;
}

void HltCtlView::setInitialZoomedAreaPos(qint64 pos)
{
    qDebug() << "HltController:: Set Initial Zoomed Area Pos";
    //slider->hide();
    //qint64 defaultZoom = 60*1000*2;
    //qint64 init = pos;
    //qint64 end = init + defaultZoom > this->getDuration() ? getDuration(): init + defaultZoom;

    setZoomedLimitValues(pos, this->getDuration());
    //updateZoomedViewOfHighlights();
}

void HltCtlView::setZoomedLimitValues(qint64 init, qint64 end)
{
    qDebug()<<"hltsController::Changing zoomed limit values";

    initZoomedTimeLine = initPlayerTimePos > init ? initPlayerTimePos : init;
    endZoomedTimeLine = endPlayerTimePos < end ? endPlayerTimePos : end;

}

void HltCtlView::updateThumbs()
{
//    if(equalZoomedLimits())
//        return;

    QVector<int> newTimes;

    newTimes.reserve(m_thumbNum);

    qreal step = (endZoomedTimeLine - initZoomedTimeLine)/m_thumbNum;
    qint64 time = initZoomedTimeLine + (qint64)(step/2.0);
    for(int i = 0; i < m_thumbNum; i++){
        if(time < 0)
            time = 0;

        newTimes.push_back(time);
        time += (qint64)step;
        //newTimes.push_back((initZoomedTimeLine+i*(endZoomedTimeLine-initZoomedTimeLine)/m_thumbNum)/1000);
    }

    //Updating zoomed limits used fer get new thumbs
    prevInitZoomedTimeLine = initZoomedTimeLine;
    prevEndZoomedTimeLine = endZoomedTimeLine;
    qDebug()<<"HltCtlView::emit signal for update Thumbnails";
    emit updateThumbSignal(newTimes);
}


QSize HltCtlView::getSliderThumbSize(){
//    qDebug()<<"sizeImagenes"<<thumbWidth*2.8<<thumbHeight*2;
    return QSize(thumbWidth*2.8,thumbHeight*2);
}

void HltCtlView::initThumbItems(){
//    qDebug()<<"HltCtlView::Init set thumbnails";
    thumbItems.resize(m_thumbNum);

    for(int i=0;i< m_thumbNum; i++){
        thumbItems[i] = scene->addPixmap(QPixmap());
        thumbItems[i]->setZValue(0.0);
    }
}

bool HltCtlView::getMergeState() const
{
    return mergeState;
}

void HltCtlView::setMergeState(bool value)
{
    mergeState = value;
}

bool HltCtlView::getSplitState() const
{
    return splitState;
}

void HltCtlView::setSplitState(bool value)
{
    splitState = value;
}


void HltCtlView::setSceneView()
{
    stretchHeight = 2;
    stretchBar = 0.08;

    //backColor = Qt::blue;
    backColor = Qt::transparent;
    barColor = QColor(140,140,140);
    hltColor = QColor(241,196,13,200);

    //Initializing scene
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0.0, 0.0,this->width(),this->height());

    //Init slider
//    slider = new sliderItem();

    //set scene
    this->setScene(scene);
    // anti-aliasing
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    //this->viewport()->installEventFilter(this);
    //this->viewport()->installEventFilter(this);
    qreal xLeft = scene->sceneRect().left();
    qreal xRight = scene->sceneRect().right();
    qreal top = scene->sceneRect().top();
    qreal bottom = scene->sceneRect().bottom();
    qDebug()<<"SLIDER HEIGHT"<< top-bottom;
    qDebug()<<scene->sceneRect().top();
    qDebug()<<scene->sceneRect().bottom();

    widthZHlt = std::abs(xRight - xLeft);
    //Adding slider
    sliderHeight = (bottom-top)*m_sliderHeightFactor;
    //sliderHeight = (bottom - top)*0.3;
    qreal m_thumbHeight = sliderHeight;
    initPosZoomedTimeLine = QPointF(xLeft,top + (m_thumbHeight/2.0) );
    endPosZoomedTimeLine = QPointF(xRight, initPosZoomedTimeLine.y());
    //m_circlesVerticalPos = (top + m_thumbHeight + bottom)/2.0;
    m_circlesVerticalPos = top + m_thumbHeight;
    qDebug()<<"Bar Width: "<<m_barWidth;

    //Inicializar lista de thumbnails (Posición)
    setThumbScene(m_thumbHeight);

    //Dibujar Fondo
    drawBackground();
}

void HltCtlView::setXmlSettings(niceXml *value)
{
    xmlSettings = value;
}


qreal HltCtlView::getZoomFactor() const
{
    return m_zoomFactor;
}

int HltCtlView::getThumbNum() const
{
    return m_thumbNum;
}

QString HltCtlView::convertSliderTimeToMMSS(qint64 diff){
    QString durationHltText;
    if(diff < 10){
        durationHltText = "0" + QString::number(diff);
    }else {
        durationHltText = QString::number(diff);
    }

    return durationHltText;
}
void HltCtlView::setThumbScene(qreal m_thumbHeight){

    //setear tamaños de thumbWidth y thumbHeight
    thumbHeight = m_thumbHeight;
    thumbWidth  = widthZHlt/((qreal)m_thumbNum);;

    initSliderPosInTimeLine = widthZHlt * 0.15;
    endSliderPosInTimeLine = widthZHlt * 0.85;

    qDebug()<<"THUMBWIDTH"<<thumbWidth;
    qDebug()<<"THUMBHEIGHT"<<thumbHeight;
    initThumbItems();
}

void HltCtlView::drawBackground()
{

    scene->setBackgroundBrush(QBrush(backColor));

    zoomedTimeLine = new timeLine();

//    qreal yPos = initPosZoomedTimeLine.y()-(sliderHeight*0.8)/2.0 + thumbHeight/2.0;
//    QPointF init(initPosZoomedTimeLine.x(),yPos);
//    QPointF end(endPosZoomedTimeLine.x(),yPos);
    zoomedTimeLine->setView(initPosZoomedTimeLine,endPosZoomedTimeLine,thumbHeight);
    scene->addItem(zoomedTimeLine);

    zoomedTimeLine->setZValue(1.0);
}

qint64 HltCtlView::getCurrentTickPos() const
{
    return currentTickPos;
}

void HltCtlView::setUpdateTickFlag(bool value)
{
    updateTickFlag = value;
}


qint64 HltCtlView::getDuration() const
{
    return (endPlayerTimePos - initPlayerTimePos);
}


void HltCtlView::verifyMerge()
{
    if(mergeState){
        qDebug()<<"HltCtlView::emit merge Hlt";
        emit mergeHlt();
    }
}

qint64 HltCtlView::getScale()
{
    return (endZoomedTimeLine - initZoomedTimeLine);
}

void HltCtlView::setParameters()
{
    if(xmlSettings != NULL)
    {
        qDebug() << "Set HltController Parameters";
        QDomElement root = xmlSettings->document.firstChildElement();

        QDomElement hltControllerSettings = xmlSettings->getClassById(root, "hltController");

        if(hltControllerSettings.isNull())
            qDebug() << "Is Null";
        else{
            //Use ThumbList
            QString par0 = xmlSettings->getClassTextElement(hltControllerSettings,"useThumbList");
            if(par0 == "NULL_PARAMETER")
                 qDebug() << "hltCtlView::setParameters::Null Value for 'useThumbList'' because this does not exist or is repeated, you need to set default parameter";
            else{
                // qDebug()<< "hltCtlView::setParameters::useThumbList :" <<par3;
                if(par0 == "false")
                    useThumbList = false;
                else
                    useThumbList = true;
            }

            //Number of Thumbnails in zoomed area
            QString par1 = xmlSettings->getClassTextElement(hltControllerSettings,"number_Of_Thumbnails");
            if(par1 == "NULL_PARAMETER")
                qDebug() << "hltCtlView::setParameters::Null Value for 'number_Of_Thumbnails'', you need to set default parameter";
            else{
                qDebug()<< "hltCtlView::setParameters::number_Of_Thumbnails :" <<par1;
                m_thumbNum = par1.toInt();
            }

            //Zoom Factor
            QString par2 = xmlSettings->getClassTextElement(hltControllerSettings,"zoomFactor");
            if(par2 == "NULL_PARAMETER")
                qDebug() << "hltCtlView::setParameters::Null Value for 'zoomFactor'', you need to set default parameter";
            else{
                qDebug()<< "hltCtlView::setParameters::zoomFactor :" <<par2;
                qreal value = (par2.toFloat());
                if(value > 0.0 && value <= 1.0)
                    m_zoomFactor = (1.0 - value)/(2.0*value);
            }

            //Font Size Sliders
            QString par3 = xmlSettings->getClassTextElement(hltControllerSettings,"slidersFontSize");
            if(par3 == "NULL_PARAMETER")
                qDebug() << "hltCtlView::setParameters::Null Value for 'slidersFontSize'', you need to set default parameter";
            else{
                qDebug()<< "hltCtlView::setParameters::slidersFontSize :" <<par3;
                m_sliderFontSize = par3.toInt();
            }

            //Width Highlight Factor
            QString par4 = xmlSettings->getClassTextElement(hltControllerSettings,"timeTickWidth");
            if(par4 == "NULL_PARAMETER")
                qDebug() << "hltCtlView::setParameters::Null Value for 'timeTickWidth'', you need to set default parameter";
            else{
                qDebug()<< "hltCtlView::setParameters::timeTickWidth :" <<par4;
                m_barWidth = par4.toFloat();
            }

            //Width Slider
            QString par5 = xmlSettings->getClassTextElement(hltControllerSettings,"sliderWidth");
            if(par5 == "NULL_PARAMETER")
                qDebug() << "hltCtlView::setParameters::Null Value for 'sliderWidth'', you need to set default parameter";
            else{
                qDebug()<< "hltCtlView::setParameters::sliderWidth :" <<par5;
                m_sliderWidth = par5.toFloat();
            }

            //Width Button Slider
            QString par6 = xmlSettings->getClassTextElement(hltControllerSettings,"sliderButtonWidth");
            if(par6 == "NULL_PARAMETER")
                qDebug() << "hltCtlView::setParameters::Null Value for 'sliderButtonWidth'', you need to set default parameter";
            else{
                qDebug()<< "hltCtlView::setParameters::sliderButtonWidth :" <<par6;
                m_sliderButtonWidth = par6.toFloat();
            }

            //Trick Icon size (same value for width and height)
            QString par7 = xmlSettings->getClassTextElement(hltControllerSettings,"trickIconSize");
            if(par7 == "NULL_PARAMETER")
                qDebug() << "hltCtlView::setParameters::Null Value for 'trickIconWidth'', you need to set default parameter";
            else{
                qDebug()<< "hltCtlView::setParameters::widthHlt_factor :" <<par7;
                int size = par7.toInt();
                m_trickIconSize = QSize(size,size);
            }

            //Slider Height Factor
            QString par8 = xmlSettings->getClassTextElement(hltControllerSettings,"sliderHeightFactor");
            if(par8 == "NULL_PARAMETER")
                qDebug() << "hltCtlView::setParameters::Null Value for 'sliderHeightFactor'', you need to set default parameter";
            else{
                qDebug()<< "hltCtlView::setParameters::sliderHeightFactor :" <<par8;
                m_sliderHeightFactor = par8.toFloat();
            }
        }
    }
}

void HltCtlView::setPlayerTimeStampData(const qint64 &init, const qint64 &end)
{
    initPlayerTimePos = init;
    endPlayerTimePos = end;
}

qint64 HltCtlView::zoomedTimeLineTotimeValue(qreal value)
{
    if(widthZHlt <= 0) return 0;

    qreal num = (qreal)(endZoomedTimeLine - initZoomedTimeLine);
    return (qint64)(value*(num/widthZHlt) + (qreal)initZoomedTimeLine);
}

bool HltCtlView::pixelPosOnRightSlider(qreal pos)
{
    //Verificar colliding right slider for pause
    if(tickItem == NULL || tickItem->isVisible() == false){
        return false;
    }

    if(myHltSlider != NULL){
        if(pos > myHltSlider->getSliderValue(RIGHT) - (myHltSlider->getSliderLenghtFactor()/2.0) && pos < myHltSlider->getSliderValue(RIGHT) + (myHltSlider->getSliderLenghtFactor()/2.0))
        {
            return true;
        }
    }

    return false;
}

bool HltCtlView::pixelPosOnLeftSlider(qreal pos)
{
    //Verificar colliding right slider for pause
    if(tickItem == NULL || tickItem->isVisible() == false){
        return false;
    }

    if(myHltSlider != NULL){
        if(pos > myHltSlider->getSliderValue(LEFT) - (myHltSlider->getSliderLenghtFactor()/2.0) && pos < myHltSlider->getSliderValue(LEFT) + (myHltSlider->getSliderLenghtFactor()/2.0))
        {
            return true;
        }
    }

    return false;
}

bool HltCtlView::pixelPosOnSlider(qreal pos)
{
    //Verificar colliding right slider for pause
    if(tickItem == NULL || tickItem->isVisible() == false){
        return false;
    }
    if(myHltSlider != NULL){
        if(pos > myHltSlider->getSliderValue(LEFT) && pos < myHltSlider->getSliderValue(RIGHT))
        {
            return true;
        }
    }

    return false;
}

bool HltCtlView::updateTick(qint64 pos)
{
    if( getDuration() < 0 ){
        qDebug() <<"hltCtlView::No updateTick";
        return true;
    }

//    qDebug() << "updateTick";
    qreal x0 = timeToZoomedTimeLine(pos);

    if(x0 >= 0){
        //Si tick esta ubicado dentro del área de pintado
        //Modelo: Actualizar dato de la posición actual

        currentTickPos = pos;
        //Vista: Dibujar
        if(!valueInZoomedZone(pos)){
            return false;
        }else {

            if(myHltSlider != NULL && myHltSlider->isVisible())
            {
                paintTimeTick(x0);
                QString currHltTime = myHltSlider->getHltTime();
                QStringList list = currHltTime.split("/");
                hltPar plTime = editedHlt.realTime;
                qint64 currInit = plTime.first;
                qint64 myHltSliderTime = currentTickPos > plTime.second ?
                          plTime.second - currInit : (qint64)currentTickPos - currInit;

                myHltSliderTime = myHltSliderTime < 0 ? 0 : myHltSliderTime;

                QString st = milliseconds_to_MS(myHltSliderTime);
                myHltSlider->setHltTime(st + "/" + list[1]);
            }
        }
    }

    //this->update();
    return true;
}

void HltCtlView::paintTimeTick(qreal pos)
{
//    qreal y0 = initPosZoomedTimeLine.y()-(sliderHeight*0.8/2.0)+m_barWidth;
//    qreal y1 = initPosZoomedTimeLine.y()+(thumbHeight*0.8/2.0)-m_barWidth;

    qreal y0 = this->zoomedTimeLine->boundingRect().top();
    qreal y1 = this->zoomedTimeLine->boundingRect().bottom();

    QPen pen(Qt::white);
    pen.setWidthF(m_barWidth);
    QLineF line(pos,y0,pos,y1);

    if(firstTick){
        tickItem = scene->addLine(line,pen);
    }else {
        tickItem->setPen(pen);
        tickItem->setLine(line);
        tickItem->show();
    }

    firstTick = false;

    pixelPosition = pos;
}

/*Retorna el valor en coordenadas de la escena de la barra de tiempo aumentada para un valor de tiempo
* Si retorna -2, entonces los parámetros de escala no estan bien definidos
*/
qreal HltCtlView::timeToZoomedTimeLine(qint64 timeValue)
{

   if(timeValue < initZoomedTimeLine)
       return 0.0;
   else if (timeValue > endZoomedTimeLine)
       return widthZHlt+1;

    qreal num = timeValue - (qreal)(initZoomedTimeLine);
    qreal den = (qreal)(endZoomedTimeLine - initZoomedTimeLine);
    qreal ret =  (num/den)*widthZHlt;
//    qDebug()<<"Pos In Zoomed Area: "<<ret;
    return ret;
}

//Retorna verdadero la posición se encuentra dentro del area zoomeada, retorna falso en caso contrario.
bool HltCtlView::valueInZoomedZone(qint64 timeValue)
{
    if(timeValue < initZoomedTimeLine)
        return false;
    else if (timeValue > endZoomedTimeLine)
        return false;

    return true;
}

bool HltCtlView::valueInPlayerTimeZone(qint64 timeValue)
{
    if(timeValue < initPlayerTimePos)
        return false;
    else if (timeValue > endPlayerTimePos)
        return false;

    return true;
}

bool HltCtlView::valueInCurrentHighlight(qint64 timeValue)
{
    if(timeValue >= editedHlt.realTime.first && timeValue <= editedHlt.realTime.second)
        return true;
    else return false;

}
/*Retorna el valor en tiempo [ms] de una posición en coordenadas del área Zoomeada
*/
qint64 HltCtlView::zoomedAreaToTime(qreal value)
{
//   if(initZoomedTimeLine < 0 || endZoomedTimeLine < 0 || getScale() > getDuration() ||
//           initPlayerTimePos < 0 || endPlayerTimePos < 0 || endZoomedTimeLine > endPlayerTimePos
//           || initZoomedTimeLine < initPlayerTimePos ){
//        qDebug()<<"Scale Parameters not defined";
//        return -2;
//    }

    if(value <= 0)
        return initZoomedTimeLine;
    else if(value >= widthZHlt)
        return endZoomedTimeLine;

    qreal num = (qreal)(endZoomedTimeLine - initZoomedTimeLine);
    qreal den = widthZHlt;
    qint64 ret =  initZoomedTimeLine + (qint64)((num/den)*value);
    //qDebug()<<"zoomedAreaToTime: "<<ret;
    return ret;
}

//Verifica los Highlights dentro del Area del Zoomeada y los dibuja
void HltCtlView::updateZoomedViewOfHighlights()
{
    //this->deleteSlider(); //Borrando líneas antiguas
    this->deleteCircles();

    //qDebug()<<"Agregando Linea al área zoomeada con lista de tamaño: "<<hlist.size();
    bool updated = addHltToZoomedTimeLine();
    if(updated){
        if(!equalZoomedLimits())
            updateThumbs();

        this->update();
    }
}

void HltCtlView::setTrickPool(const QMultiMap<QString,hltPar> &tp){
    trickPool = tp;
}

//Agregar el Highlight en el área zoomeada si este se ve
bool HltCtlView::addHltToZoomedTimeLine()
{
    highlightData hdata = editedHlt;

    //Si el highlight se encuentra fuera de los límites, entonces no se agrega.
    if((hdata.realTime.first < initZoomedTimeLine && hdata.realTime.second < initZoomedTimeLine) ||
       (hdata.realTime.first > endZoomedTimeLine && hdata.realTime.second > endZoomedTimeLine)){
        if(myHltSlider != NULL && myHltSlider->isVisible()){
            myHltSlider->hide();
        }
        return false;
    }

    qreal i = timeToZoomedTimeLine(hdata.realTime.first);
    qreal e = timeToZoomedTimeLine(hdata.realTime.second);

    bool lcc = true;
    bool rcc = true;

    if(e <= 0.0 || i >=  widthZHlt || i == -2 || e == -2)
        return false;

    if(i == 0.0) {
        lcc = false;
    }

    if(e >= widthZHlt) {
        rcc = false;
    }

    QLineF line(QPointF(i,zoomedTimeLine->boundingRect().center().y()),QPointF(e,zoomedTimeLine->boundingRect().center().y()));
//    qDebug()<<"Agregando Highlight N° "<<idx;
    updateSlider(line, lcc, rcc);
    insertItemCircles();

    return true;
}



void HltCtlView::splitByTick()
{
    if(splitState)
    {
        qint64 pos = zoomedTimeLineTotimeValue(pixelPosition);
        qDebug()<<"HltCtlView::Attempt to split at" << milliseconds_to_HMS(pos);
        if(pixelPosOnSlider(pixelPosition)){
            qDebug()<<"Right position to make split";
            emit splitHltTree(pos);
        }
        else
            qDebug()<<"Wrong position to make split";
    }
}

hltPar HltCtlView::getEditedHlt() const
{
    return editedHlt.realTime;
}

void HltCtlView::setPath(const QString &value)
{
    path = value;
}

qint64 HltCtlView::getMinimalZoom() const
{
    return minimalZoom;
}

void HltCtlView::updateEditedHltData(highlightData m_editedHlt)
{
    qDebug()<<"HltCtlView::Update Highlight Data";
    editedHlt = m_editedHlt;
    //editedHlt = m_editedHlt;
    //Update View
    //updateZoomedViewOfHighlights();
}


bool HltCtlView::hltParInZoommedArea(hltPar t)
{
    bool ret = t.first >= initZoomedTimeLine && t.second <= endZoomedTimeLine && t.first < endZoomedTimeLine && t.second > initZoomedTimeLine? true : false;
    return ret;
}

//idea: obtener zoom de hlist ( si es -1 inicializarlo en alguna parte )
//actualizar vista de hltview con ese zoom  y seleccionar ese highlight:
void HltCtlView::selectCurrentHlt()
{
    qDebug()<<"HltCtlView::SelectcurrentHLT";

    hltPar plTime = editedHlt.realTime;
//        if(!hltParInZoommedArea(plTime)){
//           hltPar newZoomedAreaLimits = getZoomLimitsFromCurrentSelect(plTime);
//           setZoomedLimitValues(newZoomedAreaLimits.first, newZoomedAreaLimits.second);
//        }else
//            setZoomedLimitValues(initZoomedTimeLine, endZoomedTimeLine);
    if(plTime.first == initZoomedTimeLine && plTime.second == endZoomedTimeLine)
        editedHlt.realTime = hltPar(plTime.first + 10000, plTime.second - 10000);
    else
        setNewZoom(plTime);

    minimalZoom = (qint64)(((qreal)(plTime.second - plTime.first))*(5.0/4.0));

    updateZoomedViewOfHighlights();
    //emit selectTickFinished(plTime.first);
    emit sliderSelectedFinished(plTime);
}

void HltCtlView::setNewZoom(hltPar plTime)
{
    qreal delta = (qreal)(plTime.second - plTime.first);
    qint64 newmargin = (qint64)(delta*(m_zoomFactor));
    setZoomedLimitValues(plTime.first - newmargin, plTime.second + newmargin);
}

hltPar HltCtlView::getZoomLimitsFromCurrentSelect(hltPar time){

    //Preguntar al modelo la posición global donde se encuentra el highlight.
    hltPar ret(initZoomedTimeLine,endZoomedTimeLine);

    qint64 currentScale = getScale();
    qint64 hltDur = time.second - time.first;

    if(currentScale > 0 && hltDur > 0 )
    {
        ret.first = time.first - (currentScale - hltDur)/2;
        ret.second = ret.first + currentScale;

    }else{
        qreal initPx = initPosZoomedTimeLine.x();
        qreal endPx = endPosZoomedTimeLine.x();
        qreal pend = (qreal)(time.second - time.first) / (endSliderPosInTimeLine - initSliderPosInTimeLine);

        ret.first =  pend*(initPx -  endSliderPosInTimeLine ) + time.second ;
        ret.second = pend*(endPx - endSliderPosInTimeLine) + time.second;
    }

    if(ret.first < 0) ret.first = 0;
    return ret;
}

void HltCtlView::updateCurrentChildTime(hltPar time)
{
    qDebug()<<"old time"<<editedHlt.realTime;
    editedHlt.realTime = time;
    qDebug()<<"new time"<<time;
}

void HltCtlView::onTimeLineSlot(qreal pos)
{
    qint64  p = zoomedTimeLineTotimeValue(pos);
    emit onTimeLine((int)p);
}

void HltCtlView::updateView()
{
    if(scene && scene->width()>0)
    {
        scene->setSceneRect(0.0, 0.0,this->width(),this->height());
        this->fitInView(0, 0, scene->width(), scene->height());
        qreal xLeft = scene->sceneRect().left();
        qreal xRight = scene->sceneRect().right();
        qreal top = scene->sceneRect().top();
        qreal bottom = scene->sceneRect().bottom();

        widthZHlt = std::abs(xRight - xLeft);
        sliderHeight = ((bottom-top)*m_sliderHeightFactor);
        //sliderHeight = (bottom - top)*0.3;
        //thumbHeight = (sliderHeight*0.64);
        thumbHeight = sliderHeight;
        initPosZoomedTimeLine = QPointF(xLeft,top + (thumbHeight/2.0) );
        endPosZoomedTimeLine = QPointF(xRight, initPosZoomedTimeLine.y());
        //m_circlesVerticalPos = (top + thumbHeight + bottom)/2.0;
        m_circlesVerticalPos = top + thumbHeight;
        //widthHlt = sliderHeight*m_widthHltFact;

        //thumbHeight = sliderHeight*0.64;
        thumbWidth  = widthZHlt/((qreal)m_thumbNum);

        initSliderPosInTimeLine = widthZHlt * 0.15;
        endSliderPosInTimeLine = widthZHlt * 0.85;

        if(zoomedTimeLine != NULL){
//            qreal yPos = initPosZoomedTimeLine.y()-(sliderHeight*0.8)/2.0 + thumbHeight/2.0;
//            QPointF init(initPosZoomedTimeLine.x(),yPos);
//            QPointF end(endPosZoomedTimeLine.x(),yPos);
            zoomedTimeLine->setView(initPosZoomedTimeLine,endPosZoomedTimeLine,thumbHeight);
        }
        //update EVENTS
        //updateZoomedViewOfHighlights();
    }
}


void HltCtlView::showEvent ( QShowEvent * event )
{
    emit showSignal();
    QGraphicsView::showEvent(event);

    updateView();
}

void HltCtlView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
}

void HltCtlView::leaveEvent(QEvent *event)
{
    emit leave();
    QGraphicsView::leaveEvent(event);
}

void HltCtlView::enterEvent(QEvent *event)
{
    emit enter();
    QGraphicsView::enterEvent(event);
}

void HltCtlView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);

    QPointF pos = this->mapToScene(event->pos());

    if(zoomedTimeLine != NULL){
        QRectF currRect = zoomedTimeLine->boundingRect();
        if(currRect.contains(pos))
        {
            onTimeLineSlot(pos.x());
        }

        trickItem *tItem = getTrickByPos(event->pos());

        if( tItem == NULL || editingSlider )
            return;
        if (!(event->buttons() & Qt::LeftButton))
            return;
        if ((event->pos() - dragStartPosition).manhattanLength()
             < QApplication::startDragDistance())
            return;

        QDrag *drag = new QDrag(this);
        QMimeData *mime = new QMimeData;
        drag->setMimeData(mime);
        //Por ahora "Single" Selection
        mime->setText(tItem->getPath() + "," + QString::number(tItem->getKey()));

        QPixmap px0 = tItem->getNormalPixmap();
        //Recorrer Lista de drag items
        if(!tricksToDrag.isEmpty() && event->modifiers() == Qt::ControlModifier)
        {
            drag->setPixmap( getMultiItemDragPixmap( pixToDrag ) );

        }else
        {
            //Set only one trick item to mime data
            drag->setPixmap(px0);
        }

        qDebug()<<"HltCtlView::Init Drag -> " << mime->text();
        drag->setHotSpot(QPoint(15, 20));

        tricksToDrag.clear();
        pixToDrag.clear();
        drag->exec();
    }

    //emit moveSliders(pos);
}

QList< QPixmap > HltCtlView::trickDragListToPixmapList()
{
    //tricksToDrag.push_front(tItem);
    QList< QPixmap > retList;
    QList< trickItem* >::iterator it = tricksToDrag.begin();
    while(it != tricksToDrag.end())
    {
        retList.push_back( (*it)->getNormalPixmap() );
        ++it;
    }

    return retList;
}

void HltCtlView::temporalSeekToPlayer(qreal pos)
{
    qint64 seekPos = zoomedTimeLineTotimeValue(pos);

    emit seekTemporalEdiToPlayer(seekPos);

    if(tickItem->isVisible())
        tickItem->hide();
}

void HltCtlView::temporalSeekToPlayerByDiff(qint64 step, qreal initialPos)
{

    qint64 seekPos = zoomedTimeLineTotimeValue(initialPos) + step;
    qDebug()<<"Temporal seek to player: "<<seekPos;

    emit seekTemporalEdiToPlayer(seekPos);

}

void HltCtlView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);

    if(editingSlider){
        emit pauseSignal_player(true);
        editingSlider = false;
    }
}

void HltCtlView::sliderPressEvent(bool isSelected)
{
    emit pauseSignal_player(true);
    editingSlider = true;
    if(!isSelected)
        emit sliderSelected();
}

void HltCtlView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);

    if(event->button() != Qt::LeftButton)
        return;

    dragStartPosition = event->pos();

    qint64  p = (qint64)zoomedTimeLineTotimeValue(QPointF(this->mapToScene(event->pos())).x());

    qreal left = myHltSlider->getSliderValue(LEFT);
    qreal right = myHltSlider->getSliderValue(RIGHT);
    qint64 init = this->zoomedTimeLineTotimeValue(left);
    qint64 end = this->zoomedTimeLineTotimeValue(right);

    if(p < init || p > end)
        return;

    trickItem *tItem = getTrickByPos(event->pos());
    if(tItem != NULL)
    {
        if( !(tricksToDrag.contains(tItem)) ){
            tricksToDrag.push_back(tItem);
            pixToDrag.push_back( tItem->getNormalPixmap() );
        }

        qDebug()<<"ChangetickPos by tick item";
        emit changeTickPos(tItem->getTimes().first);

    }else if(getSliderByPos(event->pos()) == NULL)
    {
        int yCoord = (int)(zoomedTimeLine->boundingRect().center().y()-((qreal)sliderHeight)/2.0);
        sliderItem *aitem = getSliderByPos(QPoint(event->pos().x(),yCoord));
        if(aitem != NULL && !aitem->selected){
            emit sliderSelected();
        }
        else{
            qDebug()<<"ChangetickPos";
            emit changeTickPos(p);
        }
    }
}

void HltCtlView::hideEvent(QHideEvent *e)
{
    emit hideSignal();
    QGraphicsView::hideEvent(e);
}

//void HltCtlView::paintEvent(QPaintEvent *event)
//{
////  qDebug()<<"Paint Hlt Ctl";

//    QGraphicsView::paintEvent(event);
//}

sliderItem* HltCtlView::getSliderByPos(QPoint pos)
{
    if ( QGraphicsItem *item = itemAt(pos) )  {

        if(item->type() == item->UserType+2)
        {
            return static_cast< sliderItem * >(item);
        }
    }
    return NULL;
}


trickItem *HltCtlView::getTrickByPos(QPoint pos)
{
    if ( QGraphicsItem *item = itemAt(pos) )  {

        if(item->type() == item->UserType+3)
        {
            return static_cast< trickItem * >(item);
        }
    }
    return NULL;
}

trickItem *HltCtlView::getTrickByKey(int p_key)
{
    QVector<trickItem *>::iterator it;

    for(it = circleItems.begin(); it != circleItems.end(); ++it)
    {
        if((*it)->getKey() == p_key)
        {
            return (*it);
        }
    }

    return NULL;
}

bool HltCtlView::pixelTransitionOnRightSlider(qreal pos)
{
    //Verificar colliding right slider for pause
    if(tickItem == NULL || tickItem->isVisible() == false || pixelPosition == pos)
        return false;

    if(myHltSlider != NULL){
        if(pixelPosition < myHltSlider->getSliderValue(RIGHT) && myHltSlider->getSliderValue(RIGHT) <= pos){
            pos = myHltSlider->getSliderValue(RIGHT);
            return true;
        }
    }

    return false;
}

QList<sliderItem *> HltCtlView::getSlidersFromList(QList<QGraphicsItem *> itemList)
{
   QList<sliderItem *> ret;
   QList<QGraphicsItem *>::iterator it;

   for(it = itemList.begin(); it != itemList.end(); ++it)
   {
       if((*it)->type() == (*it)->UserType+2)
       {
           ret.push_back(static_cast< sliderItem * >((*it)));
       }
   }

   return ret;
}

qreal HltCtlView::getMaxZValueFromSliders(QList<sliderItem*> sliderList)
{
    QList<sliderItem *>::iterator it;
    qreal max = 2.0;
    for(it = sliderList.begin(); it != sliderList.end(); ++it)
    {
       max = (*it)->zValue() > max ? (*it)->zValue() : max;
    }

    return max;
}

void HltCtlView::initSlider()
{
    myHltSlider = new sliderItem();

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(8.0); //Can be set by niceSettings
    effect->setOffset(4.0); //Can be set by niceSettings
    myHltSlider->setGraphicsEffect( effect );
    myHltSlider->setZValue(1.9);

    connect(myHltSlider, SIGNAL(sliderPress(bool)),this,SLOT(sliderPressEvent(bool)));
    connect(myHltSlider, SIGNAL(sliderChange(bool )), this,SLOT(sliderChangeSlot(bool )));
    connect(myHltSlider,SIGNAL(movingSlider()),this, SLOT(sliderMoveSlot()));
    connect(myHltSlider, SIGNAL(modifyTimeByPos(qreal)), this,SLOT(temporalSeekToPlayer(qreal)));
    connect(myHltSlider, SIGNAL(modifyTime(qint64, qreal)),this,SLOT(temporalSeekToPlayerByDiff(qint64,qreal)));

    scene->addItem(myHltSlider);

    myHltSlider->hide();
}

void HltCtlView::updateSlider(QLineF hlt, bool lcc, bool rcc)
{

    if(myHltSlider == NULL){
        this->initSlider();
    }
    //myHltSlider = new sliderItem();

//    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
//    effect->setBlurRadius(8.0); //Can be set by niceSettings
//    effect->setOffset(4.0); //Can be set by niceSettings
//    myHltSlider->setGraphicsEffect( effect );

    myHltSlider->selected = true;
    myHltSlider->line = hlt;
    myHltSlider->leftContained = lcc;
    myHltSlider->rightContained = rcc;

    //Can be set by niceSettings (radius)
    qreal buttonRatio = 0.15;
    myHltSlider->setSliderGeometry(getHighlightRectFromLine(hlt), buttonRatio, m_sliderWidth, m_sliderButtonWidth);

    //scene->addItem(myHltSlider);
    myHltSlider->setLocalLimits();

    hltPar slTime = editedHlt.realTime;
    QString hltTimeStr = "00/"+milliseconds_to_MS(slTime.second - slTime.first);

    myHltSlider->setFontSize(m_sliderFontSize);
    myHltSlider->setHltTime(hltTimeStr);
    myHltSlider->setTimes(slTime.first,slTime.second);
    myHltSlider->setPath(path);

    //myHltSlider->update();

    if(!myHltSlider->isVisible())
        myHltSlider->show();
}

void HltCtlView::insertCircle(qreal x, qreal y, QString trick, bool isActive, int idx)
{
    //QGraphicsEllipseItem *circle = scene->addEllipse(x,y,w,w,QPen(QColor(241,196,13,200)),QColor(241,196,13,200));
    QPixmap p;
    QString path1 = trickToPixmapPath(trick) + ".png";
    QString path2 = trickToPixmapPath(trick) + "-activo.png";

    trickItem *circle = new trickItem();
    circle->setKey(idx);
    //Load first path (normal icon)
    p.load(path1);

    bool resizeIcon = (m_trickIconSize != QSize(-1,-1));

    if(!resizeIcon)
        circle->setNormalPixmap(p, !isActive);
    else
        circle->setNormalPixmap(p.scaled(m_trickIconSize), !isActive);

    //Load second path (selected icon)
    p.load(path2);
    if(!resizeIcon)
        circle->setSelectedPixmap(p, isActive);
    else
        circle->setSelectedPixmap(p.scaled(m_trickIconSize), isActive);

    scene->addItem(circle);
    circle->setPos(x,y);
    circle->setZValue(2.0);
    //circle->setFlag( QGraphicsItem::ItemIgnoresTransformations);
    circle->setPath(this->path);
    circleItems.push_back(circle);
}

void HltCtlView::insertItemCircles(){

    QMultiMap<QString,hltPar>::iterator itTwo;
    int i = 0;

    qreal ypos = m_circlesVerticalPos;
    qreal w = (this->sceneRect().height() - m_circlesVerticalPos)*2.0;
    qreal h = w;
    //m_trickIconSize = QSize(w,h);
    m_trickIconSize = QSize(-1,-1);

    for(itTwo = trickPool.begin(); itTwo != trickPool.end(); ++itTwo, i++)
    {
        hltPar data = itTwo.value();
        qDebug()<<data;
        qint64 pos = (qint64)((data.first + data.second)/2);
        if(!valueInZoomedZone(pos))
            continue;

        QString trick = itTwo.key();
        qreal xpos = timeToZoomedTimeLine( pos );
//        qreal ypos = m_circlesVerticalPos;
//        qreal w = m_trickIconSize.width();
//        qreal h = thumbHeight/4.0;
        insertCircle(xpos - (w/2.0),ypos - (h/2.0), trick, false,i);
        circleItems.last()->setTimes(data);

//        qreal cirWidth = (qreal)circleItems.last()->boundingRect().width();
//        if(cirWidth > myHltSlider->shape().boundingRect().width())
//            circleItems.last()->hide();
    }
}

void HltCtlView::deleteSlider()
{
    if(myHltSlider != NULL){
    //                qDebug() <<"Remove Item "<<(*it)->key;
        if(scene != NULL)
            scene->removeItem(myHltSlider);

        myHltSlider->deleteLater();
    }
}

void HltCtlView::deleteCircles()
{
    if(!circleItems.empty())
    {
//        qDebug() <<"HltCtlView:: Remove Highlight Items";
        //Recorrer rect graphics items para borrarlos de la escena.
        //QVector<QGraphicsEllipseItem *>::iterator it;
        QVector<trickItem *>::iterator it;
        for(it = circleItems.begin() ; it != circleItems.end() ; ++it)
        {
            if((*it) != NULL){
//                qDebug() <<"Remove Item "<<(*it)->key;
                if(scene != NULL)
                    scene->removeItem((*it));
                delete (*it);
            }
        }
        circleItems.clear();
    }
}

QRectF HltCtlView::getHighlightRectFromLine(QLineF m_line)
{
    QPointF initPos(m_line.x1(), this->zoomedTimeLine->y());

    return QRectF(initPos,QSizeF(m_line.length(),sliderHeight));
}

void HltCtlView::sliderMoveSlot()
{
    if(myHltSlider != NULL){
        qreal left = myHltSlider->getSliderValue(LEFT);
        qreal right = myHltSlider->getSliderValue(RIGHT);
        qint64 start = zoomedTimeLineTotimeValue(left);
        qint64 end = zoomedTimeLineTotimeValue(right);
        QString currHltTime = myHltSlider->getHltTime();
        QString currTime = (currHltTime.split("/"))[0];
        QString hltTime = currTime+"/"+milliseconds_to_MS(end - start);
        myHltSlider->setHltTime(hltTime);

        emit sliderModified(hltPar((qint32)start,(qint32)end));
        emit sliderSelectedFinished(hltPar((qint32)start,(qint32)end));
    }
}

void HltCtlView::sliderChangeSlot(bool limitReached)
{
    //Verificar si colisiona con algún slider
    qDebug()<< "Alguno de los slider cambio";
    if(myHltSlider != NULL){
        QGraphicsItem *item = static_cast<QGraphicsItem*>(myHltSlider);

        if(item != NULL){
            QList<QGraphicsItem*> colItems = scene->collidingItems(item);
            QList<sliderItem*> colSliders = this->getSlidersFromList(colItems);

            if(colSliders.size() == 0)
                item->setZValue(1.9);
        }

        qreal left = myHltSlider->getSliderValue(LEFT);
        qreal right = myHltSlider->getSliderValue(RIGHT);
        qint64 init = this->zoomedTimeLineTotimeValue(left);
        qint64 end = this->zoomedTimeLineTotimeValue(right);

        myHltSlider->setTimes(init, end);
        emit highlightModified(hltPar(init,end));

        if(limitReached){
            setNewZoom(hltPar(init,end));
            updateZoomedViewOfHighlights();
            //Hide tick item when limit is reached, for visual purpose
            if(tickItem && tickItem->isVisible())
                tickItem->hide();
        }
    }
}

void HltCtlView::durationChanged(qint64 init, qint64 end)
{

    //Set Global parameters
    this->setPlayerTimeStampData(init, end);
    setInitialZoomedAreaPos(init);
}

QDebug operator<<(QDebug dbg, const sliderItem &item)
{
    Q_UNUSED(item)
    //Imprime posición corregida del slider
    return dbg.maybeSpace();
}
