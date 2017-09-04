#include "UpWardComboBox.h"
#include <QDebug>
#include <QPainter>

upWardComboBox::upWardComboBox(QWidget *parent):
    QComboBox(parent)
{

}

void upWardComboBox::showPopup() {
    QComboBox::showPopup();
    QWidget *popup = this->findChild<QFrame*>();

    qDebug()<<"ComboBox:";
    qDebug()<<"Pos: "<<this->mapToParent(QPoint(this->x(),this->y()));;
    qDebug()<<"WIDTH: "<<this->width();
    qDebug()<<"HEIGHT: "<<this->height();

    qDebug()<<"Popup:";
    if(popup != NULL){
         qDebug()<<"X: "<<popup->x();
         qDebug()<<"Y: "<<popup->y();
         qDebug()<<"WIDTH: "<<popup->width();
         qDebug()<<"HEIGHT: "<<popup->height();
#ifdef __APPLE__

#else
         popup->move(popup->x(),popup->y()-(this->height()+popup->height()));
#endif
    }
}

void upWardComboBox::upBoxIndex()
{
    int newindex = this->currentIndex()+1;
    if(newindex >= this->count())
        newindex = 0;

    this->setCurrentIndex(newindex);
}

void upWardComboBox::downBoxIndex()
{
    int newindex = this->currentIndex()-1;
    if(newindex <0)
        newindex = this->count()-1;

    this->setCurrentIndex(newindex);
}
void upWardComboBox::leaveEvent(QEvent *e)
{
    emit leave();
    QComboBox::leaveEvent(e);
}

