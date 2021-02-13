
#include "mainwindow.h"
#include <iostream>
#include <string>
#include <QtWidgets>



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  //
  // construct image_widget

  image_widget_ = new QLabel;
  image_widget_->setBackgroundRole(QPalette::Base);
  image_widget_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  image_widget_->setScaledContents(true);
  image_widget_->setMinimumSize(300, 300);

  create_actions();
  create_menus();

  setCentralWidget(image_widget_);
  resize(QGuiApplication::primaryScreen()->availableSize() * 2 / 5);

}


void MainWindow::create_actions() {
  //
  action_checker_board_image_ = new QAction(tr("Checker Board Image"), this);
  connect(action_checker_board_image_, SIGNAL(triggered()), this, SLOT(slot_load_checker_board_image()));

  action_triangle_image_ = new QAction(tr("Triangle Image"), this);
  connect(action_triangle_image_, SIGNAL(triggered()), this, SLOT(slot_load_triangle_image()));

  action_exit_ = new QAction(tr("E&xit"), this);
  action_exit_->setShortcut(tr("Ctrl+Q"));
  connect(action_exit_, SIGNAL(triggered()), this, SLOT(slot_exit()));

  action_about_ = new QAction(tr("&About"), this);
  connect(action_about_, SIGNAL(triggered()), this, SLOT(slot_about()));
}

void MainWindow::slot_exit() {
  close();
}


void MainWindow::slot_about() {
    QMessageBox::about(this, tr("About Image Viewer"),
            tr("<p>The user can select several images to be displayed on the screen</p>"));
}


void MainWindow::create_menus() {
  menu_open_ = new QMenu(tr("&Open"), this);

  menu_open_->addAction(action_checker_board_image_);
  menu_open_->addAction(action_triangle_image_);
  menu_open_->addAction(action_exit_);


  menu_help_ = new QMenu(tr("&Help"), this);
  menu_help_->addAction(action_about_);

  menuBar()->addMenu(menu_open_);
  menuBar()->addMenu(menu_help_);
}




//
// Checker Board Triangle
//
const int checker_board_width = 600;
const int checker_board_height = 600;
const int triangle_width = 600;
const int triangle_height = 600;

const double t_x1 = 0.64;
const double t_y1 = 0.33;
const double t_x2 = 0.3;
const double t_y2 = 0.6;
const double t_x3 = 0.15;
const double t_y3 = 0.06;

double MainWindow::pixelToReal(int px, bool type){
    double real = 0.0;
    if(type){ real = (double)px/600.0; }
    else{ real = 1.0-(double)px/600.0;}

    return real;
}

int MainWindow::realToPixel(double px, bool type){
    int pixel = 0;
    if(type){ pixel = px*600; }
    else{ pixel = (1.0-px)*600;}

    return pixel;
}

double MainWindow::calcLambda(double x, double y, int type){
    double lambda = 0.0;
    double realX = pixelToReal(x,true);
    double realY = pixelToReal(y, false);

    // this differentiation could be slightly more sophisticated :D
    if(type == 1){
        // calculate lambda1
        lambda = ((t_y2-t_y3)*(realX-t_x3)+(t_x3-t_x2)*(realY-t_y3))/((t_y2-t_y3)*(t_x1-t_x3)+(t_x3-t_x2)*(t_y1-t_y3));
    }else{
        // calculate lambda2
        lambda = ((t_y3-t_y1)*(realX-t_x3)+(t_x1-t_x3)*(realY-t_y3))/((t_y2-t_y3)*(t_x1-t_x3)+(t_x3-t_x2)*(t_y1-t_y3));
    }
    return lambda;
}


void MainWindow::slot_load_triangle_image() {
    QImage triangle_image(triangle_width, triangle_height, QImage::Format_RGB32);
    QPainter painter(&triangle_image);
    painter.fillRect(triangle_image.rect(), Qt::black);
    QPen pen(QColor(51,204,102), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);

    // draw frame
    painter.drawRect(triangle_image.rect().x(),triangle_image.rect().y(),triangle_width-1,triangle_height-1);

    pen.setWidth(1);
    pen.setStyle(Qt::DashLine);
    painter.setPen(pen);

    // draw grid
    for (int x = 60; x < triangle_width; x+=60) {
        QLineF line(x, 0.0,x,triangle_height);
        painter.drawLine(line);
    }

    for (int y = 60; y < triangle_height; y+=60) {
        QLineF line(0.0, y,triangle_width,y);
        painter.drawLine(line);
    }

    // create virtual triangle in Pixel world
    QPoint redP(realToPixel(t_x1,true),realToPixel(t_y1,false));
    QPoint greenP(realToPixel(t_x2,true),realToPixel(t_y2,false));
    QPoint blueP(realToPixel(t_x3,true),realToPixel(t_y3,false));

    QList<QPoint> points = {redP, greenP, blueP};
    QPolygon triangle(points);

    // get bounding box around the object to scan only relevant pixels
    int leftBounds = triangle.boundingRect().left();
    int rightBounds = triangle.boundingRect().right();
    int topBounds = triangle.boundingRect().top();
    int bottomBounds = triangle.boundingRect().bottom();

    double lambda1;
    double lambda2;
    double lambda3;
    int redVal;
    int greenVal;
    int blueVal;

    for (int x = leftBounds+1; x < rightBounds; x++) {
        for (int y = topBounds+1; y < bottomBounds; y++) {
            if(triangle.containsPoint(QPoint(x,y),Qt::OddEvenFill)){
                // we are inside the triangle
                lambda1 = calcLambda(x,y,1);
                lambda2 = calcLambda(x,y,2);
                lambda3 = 1 - lambda1 - lambda2;

                // we should not receieve negative values, but there might be some tricky rounding in the air
                if(lambda3 > 0){
                    // set RGB values
                    redVal = 255*lambda1;
                    greenVal = 255*lambda2;
                    blueVal = 255*lambda3;
                }

                // Finally, color pixel :)
                triangle_image.setPixel(x, y, qRgb(redVal, greenVal, blueVal));
            }
        }
    }


  //
  // You probably don't want to change these lines
  //

  image_widget_->setPixmap(QPixmap::fromImage(triangle_image));
  image_widget_->setFixedSize(triangle_width, triangle_height);
  adjustSize();
}

void MainWindow::slot_load_checker_board_image() {

  // The ~10 lines of code below must be changed with your code 
  // Create a QImage of required size
  // Draw a simple black/white checker board

  QImage checker_board_image(checker_board_width, checker_board_height, QImage::Format_RGB32);
  QPainter painter(&checker_board_image);
  painter.fillRect(checker_board_image.rect(), Qt::black);
  for (int y = 0; y < checker_board_height; ++y) {
    for (int x = 0; x < checker_board_width; ++x) {
      if ((x ^ y) & 0x20) {
        checker_board_image.setPixel(x, y, qRgb(255, 255, 255));
      }
    }
  }

  //
  // You probably don't want to change these lines 
  //

  image_widget_->setPixmap(QPixmap::fromImage(checker_board_image));
  image_widget_->setFixedSize(checker_board_width, checker_board_height);
  adjustSize();
}
