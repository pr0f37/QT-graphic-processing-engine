#ifndef LSSIMAGE_H
#define LSSIMAGE_H
#include <QString>
#include <QLinkedList>
#include <QLinkedListIterator>
#include <vector>
#include <QMap>
#include "lssdescriptor.h"


using namespace std;
class LssImage
{
public:
    LssImage();
    LssImage(int, int, QString *, int *, QLinkedList<LssDescriptor*> *);
    LssImage(int, int, QString *, QLinkedList<LssDescriptor*> *);
    LssImage(int, int, QString *, vector<float> *);
    LssImage(QString *, vector<float> *);
    LssImage(LssImage *);
    QString toString(short);

    int _id;
    int _class;
    QString * _name;
    int * _bthHistogram;
    QLinkedList<LssDescriptor*> * _descriptors;
    QMap<uint, int> _distances;
};

#endif // LSSIMAGE_H
