#include "view.h"

View::View(const QString &images, QWidget *parent) :
    QGraphicsView(parent)
{
    imageTable = new QSqlTableModel(this);
    imageTable->setTable(images);
    imageTable->select();

    scene = new QGraphicsScene(this);

    setScene(scene);
    _quarter = "bst_key";
    addItems("0", _quarter);
}

void View::addItems(const QString &key, const QString &columnName)
{
    QStringList keys = key.split(",");
    QString message;

    message = "SELECT * FROM images WHERE " + columnName + " IN (" + key + ") order by bst_key ASC";
    QSqlQuery query(message);
    QSqlRecord record = query.record();
    int idCol = record.indexOf("id");
    int fileCol = record.indexOf("file");
    int keyCol = record.indexOf(_quarter);

    int imageOffset = 150;
    int leftMargin = 40;
    int topMargin = 15;
    int j = 0;
    int i = 0;
    while (query.next())
    {
        ImageItem *image;
        QGraphicsTextItem *label;

        int id = query.value(idCol).toInt(); // read image id from database
        QString file = query.value(fileCol).toString(); // read image filename from database
        QString code  = query.value(keyCol).toString(); // read image code from database
        int columnOffset = ((i % 3) * 37);
        int x = ((i % 3) * imageOffset) + leftMargin + columnOffset;
        if ((i != 0) && (i % 3 == 0)) j++;
        int y = ((j) * imageOffset) + topMargin;

        image = new ImageItem(id, QPixmap(file));
        image->setData(0, i);
        image->setPos(x, y);
        scene->addItem(image);
        label = scene->addText(code);
        QPointF labelOffset((150 - label->boundingRect().width()) / 2, 120.0);
        label->setPos(QPointF(x, y) + labelOffset);

        i++;
    }
    scene->setSceneRect(0, 0, 600, 150 * (j+1));
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
    if (QGraphicsItem *item = itemAt(event->pos())) {
        if (ImageItem *image = qgraphicsitem_cast<ImageItem *>(item))
            showPreview(image);
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void View::showPreview(ImageItem *image)
{
    int id = image->id();
    if (id < 0 || id >= imageTable->rowCount())
        return;

    InfoWindow *window = findWindow(id);
    if (window && window->isVisible()) {
        window->raise();
        window->activateWindow();
    } else if (window && !window->isVisible()) {
        window->show();
    } else {
        InfoWindow *window;
        window = new InfoWindow(id, imageTable, this);

        window->show();
        informationWindows.append(window);
    }
}

InfoWindow* View::findWindow(int id)
{
    QList<InfoWindow*>::iterator i, beginning, end;

    beginning = informationWindows.begin();
    end = informationWindows.end();

    for (i = beginning; i != end; ++i) {
        InfoWindow *window = (*i);
        if (window && (window->id() == id))
            return window;
    }
    return 0;
}

void View::updateItems(const QString key)
{
    scene->clear();
    addItems(key, _quarter);
}

void View::updateItemsBth(const QString id)
{
    scene->clear();
    addItems(id, "id");
}

void View::updateItemsSift(const QString id)
{
    scene->clear();
    addItems(id, "id");
}

void View::setBstQuarter(const int quarter)
{
    switch(quarter)
    {
    case 0:
        _quarter = "bst_key";
        break;
    case 1:
        _quarter = "bk_first";
        break;
    case 2:
        _quarter = "bk_second";
        break;
    case 3:
        _quarter = "bk_third";
        break;
    case 4:
        _quarter = "bk_fourth";
        break;
    }
}


