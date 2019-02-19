#ifndef LSSCLASSCONTAINER_H
#define LSSCLASSCONTAINER_H
#include "lssdescriptor.h"
#include <QMap>
class LssClassContainer
{
public:
    LssClassContainer(int);
    void addClassToContainer(LssDescriptor &);

    int _class;
    int _power;
    QMap<int, LssDescriptor *> _descriptors;
};

#endif // LSSCLASSCONTAINER_H
