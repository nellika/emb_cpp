
#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <vector>

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
QT_END_NAMESPACE


class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow() = default;

  QLabel *image_widget_;

private slots:
  void slot_exit();
  void slot_about();
  void slot_load_triangle_image();
  void slot_load_checker_board_image();
  double calcLambda(double x, double y, int type);
  double pixelToReal(int px, bool type);
  int realToPixel(double px, bool type);

private:
  void create_menus();
  void create_actions();

  QAction *action_checker_board_image_;
  QAction *action_triangle_image_;
  QAction *action_about_;
  QAction *action_exit_;

  QMenu *menu_open_;
  QMenu *menu_help_;
};

#endif  // MAINWINDOW_H
