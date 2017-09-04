#ifndef TRICKITEM_H
#define TRICKITEM_H

#include <QGraphicsPixmapItem>
#include "swtypes.h"
class trickItem : public QGraphicsPixmapItem
{
    Q_INTERFACES(QGraphicsPixmapItem)
public:

    explicit trickItem(QGraphicsItem *parent = 0);
    explicit trickItem(const QPixmap &pixmap, QGraphicsItem *parent = 0);
    trickItem(const trickItem &item);

    trickItem& operator=(const trickItem&);

    bool operator==(const trickItem&);

    enum { Type = UserType + 3 };

    int type() const
    {
       // Enable the use of qgraphicsitem_cast with this item.
       return Type;
    }

    int getKey() const;
    void setKey(int value);

    bool getIsSelected() const;
    void setIsSelected(bool value);

    QPixmap getNormalPixmap() const;
    void setNormalPixmap(const QPixmap &value, bool set = false);

    QPixmap getSelectedPixmap() const;
    void setSelectedPixmap(const QPixmap &value, bool set = false);

    hltPar getTimes() const;
    void setTimes(const hltPar &value);

    QString getPath() const;
    void setPath(const QString &value);

private:

    QPixmap normalPixmap;
    QPixmap selectedPixmap;

    int key;

    bool isSelected;

    hltPar times;

    QString path;

    void init();
protected:

    void hoverEnterEvent(QGraphicsSceneHoverEvent *e) Q_DECL_OVERRIDE;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *e) Q_DECL_OVERRIDE;
//    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
//    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

};

#endif // TRICKITEM_H
