#ifndef SIFTCLASSCONTAINER_H
#define SIFTCLASSCONTAINER_H
#include <QMap>
#include "siftdescriptor.h"
class SiftClassContainer
{
public:
    SiftClassContainer(int);
    void addSiftToContainer(SiftDescriptor &);
    int _class;
    int _power;

    QMap<int, SiftDescriptor *> _descriptors;

};

#endif // SIFTCLASSCONTAINER_H
