#include "siftclasscontainer.h"

SiftClassContainer::SiftClassContainer(int aClass)
{
    this->_class = aClass;
    this->_power = 0;
}

void SiftClassContainer::addSiftToContainer( SiftDescriptor &desc)
{
    if (this->_class == desc._class)
    {
        this->_descriptors.insert(_descriptors.count(), &desc);
        this->_power++;
    }

}
