#ifndef SIFTIMAGE_H
#define SIFTIMAGE_H
#include <QLinkedList>
#include <QString>
#include <QMap>
#include "siftdescriptor.h"
extern "C" {

    #include "imgfeatures.h"

}

class SiftImage
{
public:
    SiftImage(const QString &, struct feature *, int n, int = 1, int = 1, int = 0, int = 0);
    SiftImage(const QString &, QLinkedList <SiftDescriptor*>, int = 1, int = 1, int = 0, int = 0);
    QLinkedList<SiftDescriptor*> _featuresList;
    QString toString();

    QString _name;
    int _id;
    int _class;
    QLinkedList<int *> _siftBth;
    QLinkedList<int *> _siftHistogram;
    QLinkedList<double *> _siftHistogramN; //sift histogram normalised to [0,1] interval
    int _width;
    int _height;
    QMap<uint, int> _distances;
    QMap<double, int> _distancesN;

};

#endif // SIFTIMAGE_H
