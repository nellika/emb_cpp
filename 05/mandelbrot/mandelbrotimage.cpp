#include "mandelbrotimage.h"
#include "qpainter.h"

void MandelbrotImage::createColorVectors(){
    ELEC4::Clamp<double> clamp_to_rgb(0., 255.);

    ELEC4::Spline interpolate_red(_xs, _yr);
    ELEC4::Spline interpolate_green(_xs, _yg);
    ELEC4::Spline interpolate_blue(_xs, _yb);

    static int vector_size = 2048;

    double x = -0.05;
    double step_size = 1.1/vector_size;
    for (int i=0; i<vector_size; i++){
        _r_spline[i] = clamp_to_rgb(interpolate_red.get_value(x));
        _g_spline[i] = clamp_to_rgb(interpolate_green.get_value(x));
        _b_spline[i] = clamp_to_rgb(interpolate_blue.get_value(x));
        x += step_size;
    }
}

void MandelbrotImage::process_sub_image(std::vector<int> current_rows){
    ELEC4::Pixel2Rect v_pixel2rect(_yc, height(), _d, 1);
    ELEC4::Pixel2Rect h_pixel2rect(_xc, width(), _d, 1.5);

    static int depth = 512;
    std::complex<double> c_0(0);
    std::complex<double> z(0);

    int n;
    std::complex<double> zn;

      for (auto y : current_rows) {
        double im_0 = v_pixel2rect(400-y);
//        double im_0=(y - 400 / 2.0) / (0.5 * _d * 400) + _yc;

        for (int x = 0; x < width(); ++x) {
            double re_0 = h_pixel2rect(x);
//            double re_0 = 1.5 * (x - 600 / 2.0) / (0.5 * _d * 600) + _xc;

            if(_julia){
//                z 0 = (x + iy), c0 = −0.4 + 0.6i
                c_0 = std::complex<double>(-0.4,0.6);
                z = std::complex<double>(re_0,im_0);
            } else {
                c_0 = std::complex<double> (re_0,im_0);
                z = std::complex<double> (0);
            }

            std::tie(n, zn) = calcMandelbrot(c_0, z, depth);

            if(n == depth){
                setPixel(x, y, qRgb(0, 0, 0));
            } else {
                double v = log2(log2(std::norm(zn)));
                int color_idx = (int)(sqrt(n + 5 - v) * 1024)%2048;
                setPixel(x,y,qRgb(_r_spline[color_idx], _g_spline[color_idx], _b_spline[color_idx]));
            }
        }
    }
}

std::tuple<int, std::complex<double>> MandelbrotImage::calcMandelbrot
            (std::complex<double> c_0, std::complex<double> z, int depth){

    int i = 0;

    while (std::norm(z) < 65536.0 && i < depth) {
        z = z*z + c_0;
        ++i;
    }

    return std::tuple(i, z);
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
    _julia = julia;

    createColorVectors();

    int bucket_nr = 0;
    size_t n_rows = height / _num_threads;
    std::vector< std::vector<int>> buckets( _num_threads );

    for (int y = 0; y < height; ++y) {
        buckets[bucket_nr].push_back(y);
        if ( (buckets[bucket_nr].size() == n_rows) && (bucket_nr < (_num_threads - 1)) ) {
            bucket_nr++;
        }
    }

    auto start = std::chrono::steady_clock::now();

    for( int i = 0; i < _num_threads; ++i ) {

        threads.emplace_back([=]() {
            process_sub_image(buckets[i]);
        });
    }

    for (auto &thread_elem : threads) {
        thread_elem.join();
    }

    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "Info: image calculated in " << ELEC4::Commify(elapsed_seconds.count()*1000000)
              << " us" << std::endl;

}
