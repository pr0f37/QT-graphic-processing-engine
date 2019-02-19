#include "infowindow.h"
#include <QTextEdit>
#include <QtSql/QSqlRelationalDelegate>
#include <QtSql/qsqlrecord.h>
InfoWindow::InfoWindow(int id, QSqlTableModel *images, QWidget *parent) : QDialog(parent)
{

    QSqlRecord record = images->record(id);
    file = record.value("file").toString();
    QPixmap imagePixmap(file);

    image = new QLabel();
    image->setPixmap(imagePixmap);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(image);
    setLayout(formLayout);

    locationId = id;

    setWindowFlags(Qt::Window);
    file.chop(4);
    file.remove(0, 7);
    setWindowTitle(tr("Image: %1").arg(file));
}

InfoWindow::InfoWindow(const QImage &img, QWidget *parent) : QDialog(parent)
{
    image = new QLabel();
    image->setPixmap(QPixmap::fromImage(img, Qt::AutoColor));

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(image);
    setLayout(formLayout);

    setWindowFlags(Qt::Window);
    setWindowTitle(tr("No title"));
}

int InfoWindow::id()
{
    return locationId;
}
