#include "lssclasscontainer.h"

LssClassContainer::LssClassContainer(int cls)
{
    this->_class = cls;
    this->_power= 0;
}

void LssClassContainer::addClassToContainer(LssDescriptor &desc)
{
    this->_descriptors.insert(_descriptors.count(), &desc);
    this->_power++;
}
