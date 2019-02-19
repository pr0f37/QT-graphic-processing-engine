#include "lssimage.h"


LssImage::LssImage()
{

}

LssImage::LssImage(int cls, int id, QString * name, int * bthHistogram, QLinkedList<LssDescriptor*> * descriptors)
{
    this->_class = cls;
    this->_id = id;
    this->_name = name;
    this->_bthHistogram = bthHistogram;
    this->_descriptors = descriptors;
}

LssImage::LssImage(int cls, int id, QString * name, QLinkedList<LssDescriptor*> * descriptors)
{
    this->_class = cls;
    this->_id = id;
    this->_name = name;
    this->_descriptors = descriptors;
}

LssImage::LssImage(int cls, int id, QString * name, vector<float> * descriptors)
{
    this->_class = cls;
    this->_id = id;
    this->_descriptors = new QLinkedList<LssDescriptor*>();
    this->_name = name;
    int counter = 1;
    QLinkedList<float> * desc = new QLinkedList<float>();

    for(vector<float>::iterator it = descriptors->begin(); it != descriptors->end(); ++it, ++counter)
    {
        desc->push_back(*it);
        if (counter % LssDescriptor::_descSize == 0)
        {
            this->_descriptors->push_back(new LssDescriptor(desc));
            desc = new QLinkedList<float>();
        }
    }
}


/* we have to remember that last value of iterator is always 0 and therefore should be omited*/
LssImage::LssImage(QString * name, vector<float> * descriptors)
{
    this->_name = name;
    this->_descriptors = new QLinkedList<LssDescriptor*>();
    int counter = 1;
    QLinkedList<float> * desc = new QLinkedList<float>();

    for(vector<float>::iterator it = descriptors->begin(); it != descriptors->end(); ++it, ++counter)
    {
        desc->push_back(*it);
        if (counter % LssDescriptor::_descSize == 0)
        {
            this->_descriptors->push_back(new LssDescriptor(desc));
            desc = new QLinkedList<float>();
        }
    }
}

LssImage::LssImage(LssImage * image)
{
    this->_class = image->_class;
    this->_name = image->_name;
    this->_bthHistogram = image->_bthHistogram;
    this->_descriptors = image->_descriptors;
}

QString LssImage::toString(short bthWordsNum)
{
    QString returnedString = "";
    returnedString += "<id>" + QString::number(this->_id) + "</id>";
    returnedString += "<class>" + QString::number(this->_class) + "</class>";
    returnedString += "<name>" + *this->_name + "</name>";
    returnedString += "<bthHistogram>";
    for(int i = 0; i < bthWordsNum; i++)
        returnedString += QString::number(*(this->_bthHistogram+i)) + ";";
    returnedString += "</bthHistogram>";
    returnedString += "<descriptors>";
    QLinkedListIterator<LssDescriptor*> it (*(this->_descriptors));
    while(it.hasNext())
        returnedString += "<descriptor>" + it.next()->toString() + "</descriptor>";
    returnedString += "</descriptors>";
    return returnedString;
}
