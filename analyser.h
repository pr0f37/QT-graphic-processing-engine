#ifndef ANALYSER_H
#define ANALYSER_H

#include <QString>
#include <QImage>
#include <QObject>
#include <QHash>
#include <QImage>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QtSql/qsqlrecord.h>
#include <QtSql/QSqlRelationalTableModel>
#include <QtSql/qsql.h>
#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlquery.h>
#include "logger.h"
#include <limits>
class ImageAnalyser;
class BthAnalyser;
class BthAreaAnalyser;
class QuarterImageAnalyser;
class SiftAnalyser;
class LssAnalyser;

class Analyser :
        public QObject
{
    Q_OBJECT

public:
    friend class SiftAnalyser;
    friend class LssAnalyser;
    Analyser(const QString&, const QString&);
    virtual void updateConfigFile();
    virtual void analyseImage(const QImage&, const QString&);
private:
    QString _fileName;
    QString _imageTableName;
};

#endif // ANALYSER_H
