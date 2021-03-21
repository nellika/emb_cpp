#include "splineimage.h"
#include <QImage>
#include "qpainter.h"

int SplineImage::realToPixel(double real, bool type){
    int pixel = 0;
    if(type){ pixel = (real+0.05)*_px_maxw/1.1; }
    else{ pixel = (265-real)*_px_maxh/275;}

    return pixel;
}

double SplineImage::pixelToReal(int px, bool type){
    double real = 0.0;
    if(type){ real = px*1.1/_px_maxw-0.05; }
    else{ real = 265-px*275/_px_maxh;}

    return real;
}

//int SplineImage::clampToRGB(double val){
//    return (val <= 0.0) ? 0 : ((val >= 255.0) ? 255 : static_cast<int>(val));
//}

//int SplineImage::clampVertical(int val) {
//    return (val < _px_min) ? 0 : ((val > _px_maxh) ? _px_maxh : val);
//}

SplineImage::SplineImage(int width, int height) : QImage(width, height, QImage::Format_RGB32)
{
    _px_maxh = height-1;
    _px_maxw = width-1;
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);
    QPen pen(Qt::white, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    ELEC4::Spline interpolate_red(_xs, _yr);
    ELEC4::Spline interpolate_green(_xs, _yg);
    ELEC4::Spline interpolate_blue(_xs, _yb);

    ELEC4::Clamp<int> clamp_to_height(0, _px_maxh);
    ELEC4::Clamp<double> clamp_to_rgb(0., 255.);

    for (int x = 0; x < width; ++x) {
        double x_real = pixelToReal(x, true);
        if(x_real < 0) x_real = 0;
        if(x_real > 1) x_real = 1;
        double yr_real = interpolate_red.get_value(x_real);
        double yg_real = interpolate_green.get_value(x_real);
        double yb_real = interpolate_blue.get_value(x_real);

        int yr = realToPixel(yr_real, false);
        int yg = realToPixel(yg_real, false);
        int yb = realToPixel(yb_real, false);

        int r = clamp_to_rgb(yr_real);
        int g = clamp_to_rgb(yg_real);
        int b = clamp_to_rgb(yb_real);

        QLineF line(x, 0.0,x,height);
        pen.setColor(qRgb(r,g,b));
        painter.setPen(pen);
        painter.drawLine(line);

        setPixel(x, clamp_to_height(yr), qRgb(255, 0, 0));
        setPixel(x, clamp_to_height(yg), qRgb(0, 255, 0));
        setPixel(x, clamp_to_height(yb), qRgb(0, 0, 255));
    }

    pen.setColor(Qt::white);
    painter.setPen(pen);

    for (auto position : _vlines) {
        int pixel_position = realToPixel(position, true);
        QLineF line(pixel_position, 0.0,pixel_position,height);
        painter.drawLine(line);
    }

    pen.setStyle(Qt::DashLine);
    painter.setPen(pen);

    for (auto position : _hlines) {
        int pixel_position = realToPixel(position, false);
        QLineF line(0.0, pixel_position,width,pixel_position);
        painter.drawLine(line);
    }
}
