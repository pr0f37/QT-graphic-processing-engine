#include "siftimage.h"


SiftImage::SiftImage(const QString &name, struct feature * feat, int n, int width, int height, int id, int cls)
{
    _name = name;
    _width = width;
    _height = height;
    _id = id;
    _class = cls;
    for (int i = 0; i < n; i++)
    {
        SiftDescriptor * sd = new SiftDescriptor(name, (feat + i)->descr, 0, (feat + i)->x, (feat + i)->y, (feat + i)->a, (feat + i)->b, (feat + i)->c, (feat + i)->ori, (feat + i)->scl);
        _featuresList << sd;
    }
}

SiftImage::SiftImage(const QString &name, QLinkedList<SiftDescriptor *> desc, int width, int height, int id, int cls)
{
    _name = name;
    _width = width;
    _height = height;
    _featuresList = desc;
    _id = id;
    _class = cls;
}

QString SiftImage::toString()
{
    QString returnedString = "";
    returnedString += "<id>" + QString::number(_id) + "</id>";
    returnedString += "<class>" + QString::number(_class) + "</class>";
    returnedString += "<name>" + _name + "</name>";
    returnedString += "<height>" + QString::number(_height) + "</height>";
    returnedString += "<width>" + QString::number(_width) + "</width>";
    returnedString += "<featuresList>";
    QLinkedListIterator<SiftDescriptor *> i(_featuresList);
    while(i.hasNext())
    {
        SiftDescriptor * feat = i.next();
        returnedString += "<descriptor>" + feat->toString() + "</descriptor>";
    }
    returnedString += "</featuresList>";
    return returnedString;
}
