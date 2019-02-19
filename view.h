#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include "infowindow.h"
#include "imageitem.h"
#include <QtSql/qsqlrecord.h>
#include <QtSql/qsqlquery.h>

class View : public QGraphicsView
{
    Q_OBJECT
public:
    View(const QString&, QWidget *parent = 0);

protected:
    void mouseReleaseEvent(QMouseEvent *event);
signals:
    void getBstQuarter();
public slots:
    void setBstQuarter(const int);
private slots:
    void updateItems(const QString);
    void updateItemsBth(const QString);
    void updateItemsSift(const QString);
private:
    void addItems(const QString&, const QString&);
    InfoWindow* findWindow(int id);
    void showPreview(ImageItem *image);

    QGraphicsScene *scene;
    QList<InfoWindow *> informationWindows;
    QSqlTableModel *imageTable;
    QString _quarter;
};

#endif // VIEW_H
