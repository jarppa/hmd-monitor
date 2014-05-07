#include "indicatorellipse.h"
#include <QtGui>

static QColor ambientToColor(int val);

QRectF IndicatorEllipse::boundingRect() const {
    //return QRectF(-50.0, -50.0, 100.0, 100.0);
    return QRectF(-(rad/2), -(rad/2), rad, rad);
}

QPainterPath IndicatorEllipse::shape() const {
     QPainterPath path;
     //path.addRect(-50, -50, 100, 100);
     path.addEllipse(-(rad/2), -(rad/2), rad, rad);
     return path;
}

IndicatorEllipse::IndicatorEllipse() :
                  rad(100)
{

}

/*IndicatorEllipse::~IndicatorEllipse() {

}*/

void IndicatorEllipse::updateIndicator(int heading, int ambient, float accz) {

    rad=static_cast<int>(accz*10.0f);
    ori=heading;
    color = ambientToColor(ambient);
    //QRectF r = boundingRect();

    //setRect(-(rad/2), -(rad/2), rad, rad);
    update();
}

void IndicatorEllipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                 QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(option);
    painter->setBrush(QBrush(color));
    painter->drawEllipse(-(rad/2), -(rad/2), rad ,rad);
    //painter->drawRect(boundingRect());
}

static QColor ambientToColor(int val) {
    QColor g(val);
    return g;
    /*if (val < 1000)
        return Qt::black;
    else if (val > 1000 && val < 5000)
        return Qt::red;
    else if (val > 5000 && val < 10000)
        return Qt::blue;
    else if (val > 10000 && val < 20000)
        return Qt::yellow;
    else
        return Qt::cyan;*/
}
