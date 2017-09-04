#ifndef ROUNDEDPOLYGON_H
#define ROUNDEDPOLYGON_H

#include <QPolygon>
#include <QPainterPath>

/*!<
 *  Rounded Polygon class based in Bazier curves.
 *  Class designed by Bojan Kverh (https://www.toptal.com/c-plus-plus/rounded-corners-bezier-curves-qpainter)
 * */

class RoundedPolygon : public QPolygon
{
public:
    RoundedPolygon()
    {    SetRadius(10); }
    void SetRadius(unsigned int iRadius)
    {    m_iRadius = iRadius; }
    const QPainterPath& GetPath();

private:
    QPointF GetLineStart(int i) const;
    QPointF GetLineEnd(int i) const;
    float GetDistance(QPoint pt1, QPoint pt2) const;
private:
    QPainterPath m_path;
    unsigned int m_iRadius;
};

#endif // ROUNDEDPOLYGON_H
