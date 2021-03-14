#ifndef MANDELBROTIMAGE_H
#define MANDELBROTIMAGE_H

#include <QImage>
#include <complex>
#include <chrono>
#include <iostream>
#include <thread>
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

    std::complex<double> _zn_256;
    int _n_256;
//    double v_pixel2rect(int px);
//    double h_pixel2rect(int px);
//    int calcMandelbrot(double Re, double Im, int depth);
    double _r_spline[2048];
    double _g_spline[2048];
    double _b_spline[2048];
    const std::vector<double> _xs{0., 0.16, 0.42, 0.6425, 0.8575};
    const std::vector<double> _yr{0., 32., 237., 215., 0.};
    const std::vector<double> _yg{7., 107., 255., 170., 10.};
    const std::vector<double> _yb{100., 183., 235., 40., 15.};

    int calcMandelbrot(std::complex<double> c, int depth);


    static const int num_threads = 8;
    void process_sub_image(int current_thread,int max_thread);
    void createColorVectors();

public:
    MandelbrotImage(int spline_width, int spline_height, double d, double x_c, double y_c);
    ~MandelbrotImage() = default;
};

#endif // MANDELBROTIMAGE_H
