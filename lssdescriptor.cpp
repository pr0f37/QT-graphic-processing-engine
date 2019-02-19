#include "lssdescriptor.h"

LssDescriptor::LssDescriptor()
{
}

LssDescriptor::LssDescriptor(QLinkedList<float> * descriptor)
{
    this->_descriptor = descriptor;
}

LssDescriptor::LssDescriptor(LssDescriptor * descriptor)
{
    this->_descriptor = descriptor->_descriptor;
}

QString LssDescriptor::toString()
{
    QString returnedString = "<descriptor>";
    QLinkedListIterator<float> it(*(this->_descriptor));
    while(it.hasNext())
        returnedString += QString::number(it.next()) + ";";
    returnedString += "</descriptor>";
    return returnedString;
}
