#ifndef MANDELBROTIMAGE_H
#define MANDELBROTIMAGE_H

#include <QImage>

class MandelbrotImage : public QImage
{
private:
    double _d;
    double _y_c;
    double _x_c;
    int _px_min = 0;
    int _px_maxw;
    int _px_maxh;
    int realToPixel(double px, bool type);
    double v_pixel2rect(int px);
    double h_pixel2rect(int px);
    int calcMandelbrot(double Re, double Im, int max_iter);
public:
    MandelbrotImage(int spline_width, int spline_height, double d, double x_c, double y_c);
    ~MandelbrotImage() = default;
};

#endif // MANDELBROTIMAGE_H
