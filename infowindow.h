#ifndef INFOWINDOW_H
#define INFOWINDOW_H

#include <QtGui>
#include <QtWidgets>
#include <QtSql/QSqlRelationalTableModel>
#include <QLabel>


class InfoWindow : public QDialog
{
    Q_OBJECT
public:
    InfoWindow(int id, QSqlTableModel *images, QWidget *parent = 0);
    InfoWindow(const QImage &img, QWidget *parent = 0);
    int id();
private:
    int locationId;
    QString displayedImage;
    QString file;
    QLabel *image;
};

#endif // INFOWINDOW_H
