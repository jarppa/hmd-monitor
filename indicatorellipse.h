#ifndef INDICATORELLIPSE_H
#define INDICATORELLIPSE_H

#include <QGraphicsItem>

class IndicatorEllipse : public QGraphicsItem
{
    //Q_OBJECT

public:
    //IndicatorEllipse(QGraphicsItem *parent = 0);
    //explicit IndicatorEllipse ();
    IndicatorEllipse ();
    QRectF boundingRect() const;
    QPainterPath shape() const;
    //~IndicatorEllipse();

    void updateIndicator(int heading, int ambient, float accz);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                     QWidget *widget);
private:
    int rad;
    int ori;
    QColor color;
};

#endif // INDICATORELLIPSE_H
