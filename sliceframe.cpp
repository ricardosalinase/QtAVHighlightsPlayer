#include "sliceframe.h"
#include <QTimer>
#include <QPushButton>
#include <QListWidget>
#include <QLayout>
#include <QPainter>
#include <QDebug>

sliceFrame::sliceFrame(QWidget *parent):
    QFrame(parent)
  , leftButton(0)
  , rightButton(0)
  , showButton(0)
  , myWidget(0)
{
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    //this->setAttribute(Qt::WA_PaintOnScreen);
    this->setAttribute(Qt::WA_ShowWithoutActivating);
    this->setFocusPolicy(Qt::NoFocus);
    this->setStyleSheet("QFrame{"
                         "background-color: qlineargradient(spread:pad, x1:0 y1:1, x2:0 y2:0, stop:0 rgba(255, 255, 255, 0), stop:1 rgba(0, 0, 0, 100) )}"
                        "QPushButton{"
                        "background-color: transparent}"
                                      );
}

void sliceFrame::leftButtonClicked()
{
    emit buttonClicked(true);
}

void sliceFrame::rightButtonClicked()
{
    emit buttonClicked(false);
}

void sliceFrame::removeCenterWidget(){
    if( myWidget == NULL ){
        qDebug()<<"sliceFrame::Widget no inserted";
        return;
    }
    disconnect(leftButton,SIGNAL(clicked()),this, SLOT(leftButtonClicked()));
    disconnect(rightButton,SIGNAL(clicked()),this, SLOT(rightButtonClicked()));
    disconnect(showButton, SIGNAL(clicked(bool)), this, SLOT(showButtonClicked(bool)));
    delete leftButton;
    delete rightButton;
    delete showButton;
    leftButton = NULL;
    rightButton = NULL;
    showButton = NULL;
    myWidget = NULL;
}

void sliceFrame::disableArrows(bool isDisabled)
{
    if(leftButton != NULL)
    {
        leftButton->setDisabled(isDisabled);
        rightButton->setDisabled(isDisabled);
    }
}

void sliceFrame::insertCenterWidget(QWidget *w, bool useMoveButtons)
{
    if( w == NULL ){
        qDebug()<<"sliceFrame::Widget no inserted";
        return;
    }

    myWidget = w;

    //Create Buttons
    if(leftButton == NULL){
    leftButton = new QPushButton(this);
    rightButton = new QPushButton(this);
    showButton = new QPushButton(this);

    connect(leftButton,SIGNAL(clicked()),this, SLOT(leftButtonClicked()));
    connect(rightButton,SIGNAL(clicked()),this, SLOT(rightButtonClicked()));
    connect(showButton, SIGNAL(clicked(bool)), this, SLOT(showButtonClicked(bool)));
    }
    //FlatProperty
    leftButton->setFlat(true);
    rightButton->setFlat(true);
    showButton->setFlat(true);
    //Set Focus Policy
    leftButton->setFocusPolicy(Qt::NoFocus);
    rightButton->setFocusPolicy(Qt::NoFocus);
    showButton->setFocusPolicy(Qt::NoFocus);

    //Set Stylesheet
    rightButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
    rightButton->setFixedSize(QSize(10,20));

    leftButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
    leftButton->setFixedSize(QSize(10,20));

    showButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
    showButton->setFixedSize(QSize(30,10));

    rightButton->setIconSize(QSize(10,20));
    leftButton->setIconSize(QSize(10,20));
    showButton->setIconSize(QSize(30,10));

    //Set Cursor Shape
    leftButton->setCursor(Qt::PointingHandCursor);
    rightButton->setCursor(Qt::PointingHandCursor);
    showButton->setCursor(Qt::PointingHandCursor);

    showButton->setCheckable(true);


//    myWidget->setParent(this);

    QHBoxLayout *hl = new QHBoxLayout();
    hl->setContentsMargins(1,1,1,1);
    QLayout *cl = this->layout();
    if(cl != NULL)
        delete cl;


    QVBoxLayout *vl = new QVBoxLayout();
    vl->setContentsMargins(1,1,1,1);
    this->setLayout(vl);

    if(useMoveButtons)
        hl->addWidget(leftButton, 0, Qt::AlignCenter);
    hl->addWidget(myWidget);

    if(useMoveButtons)
        hl->addWidget(rightButton, 0, Qt::AlignCenter);

    hl->setSpacing(3);
    vl->insertLayout(0,hl);
    vl->insertWidget(1,showButton,0,Qt::AlignHCenter | Qt::AlignTop);
    //vl->setMargin(20);

    showButton->setIcon(getButtonIcon(showButton->iconSize(), centerzone, true));

    if(!useMoveButtons){
        //hide buttons
        leftButton->hide();
        rightButton->hide();
    }else
    {
        //Get icons
        leftButton->setIcon(getButtonIcon(leftButton->iconSize(),leftzone));
        rightButton->setIcon(getButtonIcon(rightButton->iconSize(),rightzone));
    }

}

void sliceFrame::showButtonClicked(bool checked)
{
    Q_ASSERT(this->layout()->count() > 0);
    QLayout *layout = this->layout()->itemAt(0)->layout();

    if(layout == NULL)
        return;

    for(int idx = 0; idx < layout->count() ; idx++)
    {
        QLayoutItem * const item = layout->itemAt(idx);
        //Note! QWidgetItem, and not QWidget!
        if(dynamic_cast<QWidgetItem *>(item)){
            if(checked){
                item->widget()->hide(); //<-- widget() will cast you a QWidget!
                this->setStyleSheet("background-color: transparent;");
            }
            else{
                this->setStyleSheet("QFrame{"
                                     "background-color: qlineargradient(spread:pad, x1:0 y1:1, x2:0 y2:0, stop:0 rgba(255, 255, 255, 0), stop:1 rgba(0, 0, 0, 100) )}"
                                    "QPushButton{"
                                    "background-color: transparent}"
                                                  );
                item->widget()->show();
            }
        }
    }

    showButton->setIcon(getButtonIcon(showButton->iconSize(), centerzone));
}

QWidget *sliceFrame::getMyWidget() const
{
    return myWidget;
}

QIcon sliceFrame::getButtonIcon(QSize rectSize, Zone czone, bool def)
{
    QPixmap px( rectSize );
    px.fill(Qt::transparent);

    QPainter painter(&px);
    painter.setRenderHints( QPainter::Antialiasing );
    QPointF p1,p2,p3;
    switch(czone)
    {
    case leftzone:
    {
        p1 = QPointF(rectSize.width(),0);
        p2 = QPointF(0,rectSize.height()/2);
        p3 = QPointF(rectSize.width(),rectSize.height());
    }
        break;
    case rightzone:
    {
        p1 = QPointF(0,0);
        p2 = QPointF(rectSize.width(),rectSize.height()/2);
        p3 = QPointF(0,rectSize.height());
    }
        break;
    case centerzone:
    {
        if(myWidget != NULL){

            if(myWidget->isVisible() || def){
                p1 = QPointF(0,rectSize.height());
                p2 = QPointF(rectSize.width()/2,0);
                p3 = QPointF(rectSize.width(),rectSize.height());
            }else
            {
                p1 = QPointF(0,0);
                p2 = QPointF(rectSize.width()/2,rectSize.height());
                p3 = QPointF(rectSize.width(),0);
            }
        }
    }
        break;
    }

    painter.setPen(QPen(Qt::white,2,Qt::SolidLine,Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(p1,p2);
    painter.drawLine(p2,p3);

    QIcon ico;
    ico.addPixmap(px, QIcon::Normal);
    ico.addPixmap(px, QIcon::Active);
    ico.addPixmap(px, QIcon::Selected);
    painter.end();
    return ico;
}
