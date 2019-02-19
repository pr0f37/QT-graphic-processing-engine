#include "imageitem.h"

ImageItem::ImageItem(int id, const QPixmap &pixmap, QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsPixmapItem(pixmap, parent)
{
    recordId = id;
//    setAcceptsHoverEvents(true);
    setAcceptHoverEvents(true);

    timeLine.setDuration(150);
    timeLine.setFrameRange(0, 150);

    connect(&timeLine, SIGNAL(frameChanged(int)), this, SLOT(setFrame(int)));
    connect(&timeLine, SIGNAL(finished()), this, SLOT(updateItemPosition()));

    adjust();
}

void ImageItem::hoverEnterEvent(QGraphicsSceneHoverEvent * /*event*/)
{
    timeLine.setDirection(QTimeLine::Forward);

    if (z != 1.0) {
        z = 1.0;
        updateItemPosition();
    }

    if (timeLine.state() == QTimeLine::NotRunning)
        timeLine.start();
}

void ImageItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * /*event*/)
{
    timeLine.setDirection(QTimeLine::Backward);
    if (z != 0.0)
        z = 0.0;

    if (timeLine.state() == QTimeLine::NotRunning)
        timeLine.start();
}

void ImageItem::setFrame(int frame)
{
    adjust();
    QPointF center = boundingRect().center();

    //translate(center.x(), center.y());
    setTransform(QTransform::fromTranslate(center.x(), center.y()), true);
    //scale(1 + frame / 330.0, 1 + frame / 330.0);
    setTransform(QTransform::fromScale(1 + frame / 330.0, 1 + frame / 330.0), true);
    //translate(-center.x(), -center.y());
    setTransform(QTransform::fromTranslate(-center.x(), -center.y()), true);
}

void ImageItem::adjust()
{
    QMatrix matrix;
    matrix.scale(150 / boundingRect().width(), 120 / boundingRect().height());
    setMatrix(matrix);
}

int ImageItem:: id()
{
    return recordId;
}

void ImageItem::updateItemPosition()
{
    setZValue(z);
}
