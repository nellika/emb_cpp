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
    static const int _num_threads = 4;

    double _d;
    double _yc;
    double _xc;
    int _px_maxw;
    int _px_maxh;
    bool _julia;

    double _r_spline[2048];
    double _g_spline[2048];
    double _b_spline[2048];
    const std::vector<double> _xs{0., 0.16, 0.42, 0.6425, 0.8575};
    const std::vector<double> _yr{0., 32., 237., 215., 0.};
    const std::vector<double> _yg{7., 107., 255., 170., 10.};
    const std::vector<double> _yb{100., 183., 235., 40., 15.};

    void createColorVectors();
    void process_sub_image(std::vector<int> current_rows);
    std::tuple<int, std::complex<double>> calcMandelbrot(std::complex<double> c_0, std::complex<double> z_0, int depth);
public:
    MandelbrotImage(int spline_width, int spline_height, double d, double x_c, double y_c, bool julia);
    ~MandelbrotImage() = default;
};

#endif // MANDELBROTIMAGE_H
