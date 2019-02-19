#include "siftdictionarysections.h"

SiftDictionarySections::SiftDictionarySections(int aClass)
{
    this->_class = aClass;
    this->_power = 0;
    double * descriptorValuesMax = new double[SiftDescriptor::_numberOfDesc];
    double * descriptorValuesMin = new double[SiftDescriptor::_numberOfDesc];
    for (int i = 0; i < SiftDescriptor::_numberOfDesc; i++)
        *(descriptorValuesMax + i) = 0;
    this->_descMax = new SiftDescriptor("Max descriptor of class " + QString::number(aClass), descriptorValuesMax, aClass);
    for (int i = 0; i < SiftDescriptor::_numberOfDesc; i++)
        *(descriptorValuesMin + i) = 255;
    this->_descMin = new SiftDescriptor("Min descriptor of class "+ QString::number(aClass), descriptorValuesMin, aClass);
}

void SiftDictionarySections::changeSection(const SiftDescriptor &desc)
{
    if (this->_class == desc._class)
    {
        this->_power++;
        for (int i = 0; i < SiftDescriptor::_numberOfDesc; i++)
        {
            if (*(this->_descMax->_descriptor + i) < *(desc._descriptor + i))
                *(this->_descMax->_descriptor + i) = *(desc._descriptor + i);
            if (*(this->_descMin->_descriptor + i) > *(desc._descriptor + i))
                *(this->_descMin->_descriptor + i) = *(desc._descriptor + i);
        }
    }
}
