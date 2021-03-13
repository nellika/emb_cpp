#include "mandelbrotimage.h"
#include "qpainter.h"

double MandelbrotImage::h_pixel2rect(int x){
    return _x_c-1.5*_d+(double(x)*3*_d/double(_px_maxw));
}

double MandelbrotImage::v_pixel2rect(int y){
    return _y_c-_d+(double(y)*2*_d/double(_px_maxh));
}

//int MandelbrotImage::calcMandelbrot(double re_0, double im_0, int depth){
//    int i = 0;
//    double re = 0;
//    double im = 0;
//    double tmp_re;

//    while (re*re + re*re < depth && i < depth) {
//        tmp_re = re*re -im*im + re_0;
//        im = 2*re*im + im_0;
//        re = tmp_re;
//        ++i;
//    }

//    return i;
//}

int MandelbrotImage::calcMandelbrot(std::complex<double> c_0, int depth){
    int i = 0;
    std::complex<double> z(0);

    while (abs(z) < 2.0 && i < depth) {
        z = pow(z, 2) + c_0;
        ++i;
    }
    return i;
}

MandelbrotImage::MandelbrotImage(int width, int height, double d, double x_c, double y_c) : QImage(width, height, QImage::Format_RGB32)
{
    _px_maxh = height-1;
    _px_maxw = width-1;
    _d = d;
    _x_c = x_c;
    _y_c = y_c;

    int n;
    int depth = 512;

    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);

    for (int x = 0; x < width; ++x) {
        double re_0 = h_pixel2rect(x);
        for (int y = 0; y < height; ++y) {
            double im_0 = v_pixel2rect(y);
            std::complex<double> c_0(re_0,im_0);

            n = calcMandelbrot(c_0, depth);
            if(n == depth) setPixel(x, y, qRgb(0, 255, 0));
            else setPixel(x, y, qRgb(255, 0, 0));
        }


    }

}
