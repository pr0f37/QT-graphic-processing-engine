#ifndef QUARTERIMAGEANALYSER_H
#define QUARTERIMAGEANALYSER_H

#include <QObject>
#include "analyser.h"
#include "imageanalyser.h"

class QuarterImageAnalyser :
        public ImageAnalyser
{
    Q_OBJECT
public:
    QuarterImageAnalyser(const QString&, const QString&);

signals:

public slots:

private:
    QHash<QString, int> _codeTreeFirst;
    QHash<QString, int> _codeTreeSecond;
    QHash<QString, int> _codeTreeThird;
    QHash<QString, int> _codeTreeFourth;
    int _choosenQuarter;
};

#endif // QUARTERIMAGEANALYSER_H
