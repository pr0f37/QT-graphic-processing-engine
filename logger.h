#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QTime>
#include <QDateTime>
#include <QLinkedList>
#include "siftdescriptor.h"
#include "siftimage.h"


class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(const QString&);
    ~Logger();
    bool logTimeAndAction(const QString&);
    bool log(const QString&);
    bool logSimple(const QString&);
    bool logSimple(QLinkedList<SiftDescriptor*>&);
    bool logSimple(QLinkedList<SiftImage*>&, short, int = 0);
    bool logSimple(SiftImage *, short, int = 0);
    bool logDistance(QLinkedList<SiftImage*>&, int);
    bool logDistance(const QString &, QMap<uint, int> &, int, int);
    bool logSimpleSIFT(QLinkedList<SiftImage*>&);
    bool logSimple(int *, int *, double *, int n, const QString &, int, int, int);
    bool logBth(const QString &, int, int *);
    bool clearLogFile();
    bool startTimeMeasuring();
private:
    QFile * _logFile;
    QTime * _timer;
    bool _isTimerOn;
};

#endif // LOGGER_H
