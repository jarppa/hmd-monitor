#include "indicatorellipse.h"
#include <QtGui>
#include <math.h>

static const double Pi = 3.14159265358979323846264338327950288419717;
static QColor ambientToColor(unsigned int val);

QRectF IndicatorEllipse::boundingRect() const {
    return QRectF(-(rad/2), -(rad/2), rad, rad);
}

QPainterPath IndicatorEllipse::shape() const {
     QPainterPath path;
     path.addEllipse(-(rad/2), -(rad/2), rad, rad);
     return path;
}

IndicatorEllipse::IndicatorEllipse() :
                  rad(50)
{

}

void IndicatorEllipse::updateIndicator(double heading, unsigned int ambient, float accz) {

    rad=50+static_cast<int>(accz*30.0f);
    ori=heading;
    color = ambientToColor(ambient);
    update();
}

void IndicatorEllipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                 QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(option);
    painter->setBrush(QBrush(color));
    painter->drawEllipse(-(rad/2), -(rad/2), rad ,rad);

    double x = sin(ori*(Pi/180))*((double)rad/2.0);
    double y = cos(ori*(Pi/180))*((double)rad/2.0);
    //qDebug() << x;
    //qDebug() << y;
    QPen p = QPen();
    p.setColor(Qt::red);
    painter->setPen(p);
    painter->drawLine(0,0,x,y);
    //painter->drawRect(boundingRect());
}

static QColor ambientToColor(unsigned int val) {
    QColor g(val);
    return g;
}
