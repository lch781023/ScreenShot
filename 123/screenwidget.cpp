
#include "screenwidget.h"
#include<QMutex>
#include<QApplication>
#include<QPainter>
#include<QDesktopWidget>
#include<QFileDialog>
#include<QEvent>
#include<QDateTime>
#include<QStringList>
#include<QMouseEvent>

# include<QScreen>
#define STRDATETIME qPrintabel (QDateTime::currentDateTime().toString("yyy-MM-dd-HH-mm-ss"));

QScopedPointer<ScreenWidget> ScreenWidget::self;
ScreenWidget *ScreenWidget::Instance(){
    if(self.isNull()){
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if(self.isNull()){
            self.reset(new ScreenWidget);
        }
    }
    return self.data();
}
ScreenWidget::ScreenWidget(QWidget * parent):QWidget(parent){
    menu= new QMenu(this);

    menu->addAction("应用当前截图",this,SLOT(SelectScreen()));
    menu->addAction("保存当前截图",this,SLOT(saveScreen()));
    menu->addAction("保存全屏截图",this,SLOT(saveFullScreen()));
    menu->addAction("截图另存为",this,SLOT(saveScreenOhter()));
    menu->addAction("全屏另存为",this,SLOT(saveFullOther()));

    menu->addAction("退出截图",this,SLOT(close()));
    screen=new Screen(QApplication::desktop()->size());
    fullScreen =new QPixmap();

}

void ScreenWidget::paintEvent(QPaintEvent *){
    int  x=screen->getLeftUp().x();
    int y=screen->getLeftUp().y();
    int  w=screen->getRightDown().x();
    int h=screen->getRightDown().y();

    QPainter painter(this);

    QPen pen;
    pen.setColor(Qt::green);
    pen.setWidth(2);
    pen.setStyle(Qt::DotLine);

    painter.setPen(pen);
    painter.drawPixmap(0,0,*backgroundScreen);

    if(w!=0&&h!=0){
        painter.drawPixmap(x,y,fullScreen->copy(x,y,w,h));
    }

    painter.drawRect(x,y,w,h);
    pen.setColor(Qt::yellow);
    painter.setPen(pen);
    painter.drawText(x+2,y-8,tr("截图范围:( %1 x %2 )-(%3 x %4 ) 图片大小：( %5 x %6").
                     arg(x).arg(y).arg(x+w).arg(y+h).arg(w).arg(h));

}

void ScreenWidget::showEvent(QShowEvent *){
    QPoint point(-1,-1);
    screen->setStart(point);
    screen->setEnd(point);
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    *fullScreen = fullScreen->grabWindow(QApplication::desktop()->winId(), 0, 0, screen->width(), screen->height());
#else
    QScreen *pscreen = QApplication::primaryScreen();
    *fullScreen = pscreen->grabWindow(QApplication::desktop()->winId(), 0, 0, screen->width(), screen->height());
#endif
    //设置透明度实现模糊背景
    QPixmap pix(screen->width(),screen->height());
    pix.fill(QColor(160,160,160,200));
    backgroundScreen=new QPixmap(*fullScreen);
    QPainter p(backgroundScreen);
    p.drawPixmap(0,0,pix);
}
void ScreenWidget::saveScreen(){
    int x = screen->getLeftUp().x();
        int y = screen->getLeftUp().y();
        int w = screen->getRightDown().x() - x;
        int h = screen->getRightDown().y() - y;

        QString fileName = QString("%1/screen_%2.png").arg(qApp->applicationDirPath()).arg(qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss")));
        fullScreen->copy(x, y, w, h).save(fileName, "png");
        close();

}void ScreenWidget::saveFullScreen()
{
    QString fileName = QString("%1/full_%2.png").arg(qApp->applicationDirPath()).arg(qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss")));
    fullScreen->save(fileName, "png");
    close();
}

void ScreenWidget::saveScreenOther()
{
    QString name = QString("%1.png").arg(qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss")));
    QString fileName = QFileDialog::getSaveFileName(this, "保存图片", name, "png Files (*.png)");
    if (!fileName.endsWith(".png")) {
        fileName += ".png";
    }

    if (fileName.length() > 0) {
        int x = screen->getLeftUp().x();
        int y = screen->getLeftUp().y();
        int w = screen->getRightDown().x() - x;
        int h = screen->getRightDown().y() - y;
        fullScreen->copy(x, y, w, h).save(fileName, "png");
        close();
    }
}

void ScreenWidget::saveFullOther()
{
    QString name = QString("%1.png").arg(qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss")));
    QString fileName = QFileDialog::getSaveFileName(this, "保存图片", name, "png Files (*.png)");
    if (!fileName.endsWith(".png")) {
        fileName += ".png";
    }

    if (fileName.length() > 0) {
        fullScreen->save(fileName, "png");
        close();
    }
}

void ScreenWidget::selectScreen()
{
    int x = screen->getLeftUp().x();
    int y = screen->getLeftUp().y();
    int w = screen->getRightDown().x() - x;
    int h = screen->getRightDown().y() - y;

    close();

    QPixmap pix = fullScreen->copy(x, y, w, h);
    emit sig_SelectImg(pix.toImage());

}

void ScreenWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (screen->getStatus() == Screen::SELECT) {
        screen->setEnd(e->pos());
    }
    else if (screen->getStatus() == Screen::MOV) {
        QPoint p(e->x() - movPos.x(), e->y() - movPos.y());
        screen->move(p);
        movPos = e->pos();
    }

    this->update();
}

void ScreenWidget::mousePressEvent(QMouseEvent *e)
{
    int status = screen->getStatus();

    if (status == Screen::SELECT) {
        screen->setStart(e->pos());
    }
    else if (status == Screen::MOV) {
        if (screen->isInArea(e->pos()) == false) {
            screen->setStart(e->pos());
            screen->setStatus(Screen::SELECT);
        }
        else {
            movPos = e->pos();
            this->setCursor(Qt::SizeAllCursor);
        }
    }

    this->update();
}

void ScreenWidget::mouseReleaseEvent(QMouseEvent *)
{
    if (screen->getStatus() == Screen::SELECT) {
        screen->setStatus(Screen::MOV);
    }
    else if (screen->getStatus() == Screen::MOV) {
        this->setCursor(Qt::ArrowCursor);
    }
}

void ScreenWidget::contextMenuEvent(QContextMenuEvent *)
{
    this->setCursor(Qt::ArrowCursor);
    menu->exec(cursor().pos());
}




















