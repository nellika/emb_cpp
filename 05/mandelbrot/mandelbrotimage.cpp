#include "mandelbrotimage.h"
#include "qpainter.h"

//double MandelbrotImage::h_pixel2rect(int x){
//    return _x_c-1.5*_d+(double(x)*3*_d/double(_px_maxw));
//}

//double MandelbrotImage::v_pixel2rect(int y){
//    return _y_c-_d+(double(y)*2*_d/double(_px_maxh));
//}

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
void MandelbrotImage::createColorVectors(){
    ELEC4::Clamp<double> clamp_to_rgb(0., 255.);

    ELEC4::Spline interpolate_red(_xs, _yr);
    ELEC4::Spline interpolate_green(_xs, _yg);
    ELEC4::Spline interpolate_blue(_xs, _yb);

    double x = -0.05;
    for (int i=0; i<2048; i++){
        _r_spline[i] = clamp_to_rgb(interpolate_red.get_value(x));
        _g_spline[i] = clamp_to_rgb(interpolate_green.get_value(x));
        _b_spline[i] = clamp_to_rgb(interpolate_blue.get_value(x));
        x=x+(1.10/2048);
    }

}

void MandelbrotImage::process_sub_image(std::vector<int> current_rows){
    ELEC4::Pixel2Rect v_pixel2rect(_yc, _px_maxh, _d, 1, 2);
    ELEC4::Pixel2Rect h_pixel2rect(_xc, _px_maxw, _d, 1.5, 3);

    int n;
    int depth = 100;

    //z 0 = (x + iy), c0 = −0.4 + 0.6i

//    for (int y = 0; y < height(); ++y) {
      for (auto y : current_rows) {
        double im_0 = v_pixel2rect(y);
        for (int x = 0; x < width(); ++x) {
            double re_0 = h_pixel2rect(x);

            std::complex<double> c_0(re_0,im_0);
            std::complex<double> z(0, 0);

            n = calcMandelbrot(c_0, z, depth);

            if(n == depth){
                setPixel(x, y, qRgb(0, 0, 0));
            } else {
//                double v=log(log(std::abs(_zn_256)*std::abs(_zn_256))/log(2))/log(2);
//                int i=1024*sqrt(_n_256+5-v);
//                printf("%d \n",i);
                setPixel(x,y,qRgb(_r_spline[x], _g_spline[x], _b_spline[x]));
            }
        }
    }
}

int MandelbrotImage::calcMandelbrot(std::complex<double> c_0, std::complex<double> z_0, int depth){
    int i = 0;
    std::complex<double> z = z_0;

    while (abs(z) < 2.0 && i < depth) {
        z = pow(z, 2) + c_0;
        if(abs(z) == 256){
            _n_256 = i;
            _zn_256 = z;
        }
        ++i;
    }
    return i;
}

MandelbrotImage::MandelbrotImage(int width, int height, double d, double xc, double yc, bool julia)
    : QImage(width, height, QImage::Format_RGB32)
{
    std::vector<std::thread> threads;
    _px_maxh = height-1;
    _px_maxw = width-1;
    _d = d;
    _xc = xc;
    _yc = yc;


    int N = 0;
    size_t n_rows = height / num_threads;
    std::vector< std::vector<int>> buckets( num_threads );
    for (int y = 0; y < height; ++y) {
        buckets[N].push_back(y);
        if ( (buckets[N].size() == n_rows) && (N < (num_threads - 1)) ) {
            N++;
        }
    }

    createColorVectors();

    auto start = std::chrono::steady_clock::now();

    for( int i = 0; i < num_threads; ++i ) {

        threads.emplace_back([=]() {
            process_sub_image(buckets[i]);
        });
    }

    for (auto &thread_elem : threads) {
        thread_elem.join();
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    printf("INFO: image calculed in %2fs\n", elapsed_seconds.count()*1000000);

//    std::cout << "Info: image calculated in " << out.str() << "us";
//    std::cout << "Info: image calculated in " << ELEC4::Commify(elapsed_seconds.count()*1000000) << "us";

}
