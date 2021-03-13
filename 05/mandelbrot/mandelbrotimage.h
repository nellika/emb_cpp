#ifndef MANDELBROTIMAGE_H
#define MANDELBROTIMAGE_H

#include <QImage>
#include <complex>
#include "elec4_util.h"

class MandelbrotImage : public QImage
{
private:
    double _d;
    double _yc;
    double _xc;
    int _px_maxw;
    int _px_maxh;
    int realToPixel(double px, bool type);
//    double v_pixel2rect(int px);
//    double h_pixel2rect(int px);
//    int calcMandelbrot(double Re, double Im, int depth);
    int calcMandelbrot(std::complex<double> c, int depth);
public:
    MandelbrotImage(int spline_width, int spline_height, double d, double x_c, double y_c);
    ~MandelbrotImage() = default;
};

#endif // MANDELBROTIMAGE_H
