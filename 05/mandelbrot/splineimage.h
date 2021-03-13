#ifndef SPLINEIMAGE_H
#define SPLINEIMAGE_H

#include <QImage>
#include <QObject>
#include <vector>
#include "elec4_util.h"

class SplineImage : public QImage
{
private:
    int _px_min = 0;
    int _px_maxw;
    int _px_maxh;
    int realToPixel(double px, bool type);
    double pixelToReal(int px, bool type);
//    int clampToRGB(double val);
//    int clampVertical(int val);
    const std::vector<double> _xs{0., 0.16, 0.42, 0.6425, 0.8575};
    const std::vector<double> _yr{0., 32., 237., 215., 0.};
    const std::vector<double> _yg{7., 107., 255., 170., 10.};
    const std::vector<double> _yb{100., 183., 235., 40., 15.};
    const std::vector<double> _vlines{0,0.16,0.42,0.6425,0.8575,1};
    const std::vector<double> _hlines{0,255};
public:
    SplineImage(int spline_width, int spline_height);
    ~SplineImage() = default;
};

#endif // SPLINEIMAGE_H
