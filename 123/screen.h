#ifndef SCREEN_H
#define SCREEN_H
#include<QSize>
#include<QPoint>
#endif // SCREEN_H
class Screen{
public:
    enum STATUS{SELECT,MOV,SET_W_H};
    //构造器：
    Screen(){}
    Screen(QSize size);
    //get、set方法：
    void setStart(QPoint pos);
    void setEnd(QPoint pos);
    QPoint getStart();
    QPoint getEnd();
    QPoint getLeftUp();
    QPoint getRightDown();
    STATUS getStatus();
    void setStatus(STATUS status);

    int width();
    int height();
    bool isInArea(QPoint pos);
    void move(QPoint p);

private:
    QPoint leftUpPos,rightDownPos;//记录截图区域左上角，右下角
    QPoint startPos,endPos;//记录鼠标开始位置、结束位置
    int maxWidth,maxHeight;//记录屏幕大小
    STATUS status;//三个状态：选择区域、移动区域、设置width和height

    //比较两位置，判断左上角，右下角
    void cmpPoint(QPoint &x,QPoint &e);
};
