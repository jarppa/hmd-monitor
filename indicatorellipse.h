#ifndef INDICATORELLIPSE_H
#define INDICATORELLIPSE_H

#include <QGraphicsItem>

class IndicatorEllipse : public QGraphicsItem
{

public:
    IndicatorEllipse ();
    QRectF boundingRect() const;
    QPainterPath shape() const;

    void updateIndicator(double heading, unsigned int ambient, float accz);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                     QWidget *widget);
private:
    int rad;
    double ori;
    QColor color;
};

#endif // INDICATORELLIPSE_H
