#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H
#include<QWidget>
#include"screen.h"
#include<QMenu>
#include<QPoint>
#include<QSize>
#endif // SCREENWIDGET_H
class ScreenWidget:public QWidget{
    Q_OBJECT
public:
    static ScreenWidget *Instance();
    explicit ScreenWidget(QWidget *parent=0);
private:
    static QScopedPointer<ScreenWidget>self;
    QMenu *menu;//右键菜单对象
    Screen *screen;//截屏对象
    QPixmap *fullScreen;//保存全屏图像
    QPixmap *backgroundScreen;//模糊背景图
    QPoint movPos;//坐标

protected:
    void contextMenuEvent(QContextMenuEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent * );
    void paintEvent(QPaintEvent *) ;
    void  showEvent(QShowEvent * );
signals:
    void sig_SelectImg(QImage img);
private slots:
    void saveScreen();
    void saveFullScreen();
    void saveScreenOther();
    void saveFullOther();
    void selectScreen();
};
