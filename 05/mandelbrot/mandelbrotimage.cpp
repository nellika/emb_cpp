#include "mandelbrotimage.h"
#include "qpainter.h"

int MandelbrotImage::realToPixel(double real, bool type){
    int pixel = 0;
    if(type){ pixel = (real+0.05)*_px_maxw/1.1; }
    else{ pixel = (265-real)*_px_maxh/275;}

    return pixel;
}

double MandelbrotImage::pixelToReal(int px, bool type){
    double real = 0.0;
    if(type){ real = px*1.1/_px_maxw-0.05; }
    else{ real = 265-px*275/_px_maxh;}

    return real;
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
void MandelbrotImage::createColorVectors(){
    ELEC4::Clamp<double> clamp_to_rgb(0., 255.);

    ELEC4::Spline interpolate_red(_xs, _yr);
    ELEC4::Spline interpolate_green(_xs, _yg);
    ELEC4::Spline interpolate_blue(_xs, _yb);

    int vector_size = 2048;

    double x = -0.05;
    double step_size = 1.1/vector_size;
    for (int i=0; i<vector_size; i++){
//        double temp = x;
//        if(x < 0) temp = 0;
//        if(x > 1) temp = 1;
        _r_spline[i] = clamp_to_rgb(interpolate_red.get_value(x));
        _g_spline[i] = clamp_to_rgb(interpolate_green.get_value(x));
        _b_spline[i] = clamp_to_rgb(interpolate_blue.get_value(x));
        x += step_size;
    }
}

void MandelbrotImage::process_sub_image(std::vector<int> current_rows){
    ELEC4::Pixel2Rect v_pixel2rect(_yc, _px_maxh, _d, 1, -2, 400);
    ELEC4::Pixel2Rect h_pixel2rect(_xc, _px_maxw, _d, 1.5, 3, 0);

    int n;
    int depth = 100;
    std::complex<double> c_0(0);
    std::complex<double> z(0);

    //z 0 = (x + iy), c0 = −0.4 + 0.6i

      for (auto y : current_rows) {
        double im_0 = v_pixel2rect(y);
        for (int x = 0; x < width(); ++x) {
            double re_0 = h_pixel2rect(x);

            if(_julia){
                c_0 = std::complex<double>(-0.4,0.6);
                z = std::complex<double>(re_0,im_0);
            } else {
                c_0 = std::complex<double> (re_0,im_0);
                z = std::complex<double> (0);
            }


            n = calcMandelbrot(c_0, z, depth);

            if(n == depth){
                setPixel(x, y, qRgb(0, 0, 0));
            } else {
                double v = log2(log2(pow(abs(_zn),2)));
                int i = (int)(sqrt(n + 5 - v) * 1024);
//                if(x == 13 && y ==24){
//                    printf("goodieee: %d, %f, %f\n", i, abs(_zn), v);
//                }
//                if(i < 0){
//                    printf("meh: %d, %f, %f, %d, %d\n", i, abs(_zn), v,x,y);
//                }
                int color_idx = i%2048;
                setPixel(x,y,qRgb(_r_spline[color_idx], _g_spline[color_idx], _b_spline[color_idx]));
            }
        }
    }
}

int MandelbrotImage::calcMandelbrot(std::complex<double> c_0, std::complex<double> z_0, int depth){
    int i = 0;
    std::complex<double> z = z_0;

    while (abs(z) <= 2.0 && i < depth) {
        z = z*z + c_0;
        ++i;
    }
    _zn = z;
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
    _julia = julia;


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
//    std::cout << "Info: image calculated in " << ELEC4::Commify(elapsed_seconds.count()*1000000) << " us" << std::endl;

}
