#include "siftdescriptor.h"

SiftDescriptor::SiftDescriptor()
{
    this->_class = 0;
}

SiftDescriptor::SiftDescriptor(const QString &name, double *descriptor, int cls , double x , double y , double a , double b , double c , double scl , double ori )
{
    this->_class = cls;
    this->_name = name;
    this->_x = x;
    this->_y = y;
    this->_a = a;
    this->_b = b;
    this->_c = c;
    this->_scl = scl;
    this->_ori = ori;
    this->_descriptor = new int[SiftDescriptor::_numberOfDesc];
    for (int i = 0; i < SiftDescriptor::_numberOfDesc; i++)
        *(this->_descriptor + i) = static_cast<int> (*(descriptor + i));
}

SiftDescriptor::SiftDescriptor(const QString &name, int *descriptor, int cls , double x , double y , double a , double b , double c , double scl , double ori )
{
    this->_class = cls;
    this->_name = name;
    this->_descriptor = descriptor;
    this->_x = x;
    this->_y = y;
    this->_a = a;
    this->_b = b;
    this->_c = c;
    this->_scl = scl;
    this->_ori = ori;
}

QString SiftDescriptor::toString()
{
    QString returnedString = "";
    returnedString += "<class>" + QString::number(_class) + "</class>";
    returnedString += "<name>" + _name + "</name>";
    returnedString += "<x>" + QString::number(_x) + "</x>";
    returnedString += "<y>" + QString::number(_y) + "</y>";
    returnedString += "<a>" + QString::number(_a) + "</a>";
    returnedString += "<b>" + QString::number(_b) + "</b>";
    returnedString += "<c>" + QString::number(_c) + "</c>";
    returnedString += "<scl>" + QString::number(_scl) + "</scl>";
    returnedString += "<ori>" + QString::number(_ori) + "</ori>";
    returnedString += "<_descriptor>";
    for (int i = 0; i < _numberOfDesc; i++)
        returnedString += QString::number( *(_descriptor + i)) + ";";
    returnedString += "</_descriptor>";
    return returnedString;
}
