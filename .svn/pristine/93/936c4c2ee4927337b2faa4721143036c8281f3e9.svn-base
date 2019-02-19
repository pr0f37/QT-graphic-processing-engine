#ifndef LSSDESCRIPTOR_H
#define LSSDESCRIPTOR_H
#include <QLinkedList>
#include <QString>

class LssDescriptor
{

public:
    LssDescriptor();
    LssDescriptor(QLinkedList<float> *);
    LssDescriptor(LssDescriptor *);
    QString toString();
    static const int _descSize = 80;
    static const int _maxLssDescVal = 1000000;
    QLinkedList<float> * _descriptor;

};

#endif // LSSDESCRIPTOR_H
