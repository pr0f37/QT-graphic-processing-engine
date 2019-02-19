#include "extendedimageanalyser.h"

ExtendedImageAnalyser::ExtendedImageAnalyser(const QString& fileName, const QString& imageTableName) :
    ImageAnalyser(fileName, imageTableName)
{
    _selectedTree = 0;
}

void ExtendedImageAnalyser::setComponent(const QString cmp)
{
    if (cmp == "RG") // set chosen component
        _component = false;
    else if (cmp == "BY")
        _component = true;
    constructTree(); // create BST tree
    _codeTreeZero = new QHash<QString, int>(*_codeTree);
    _codeTreeFirst = new QHash<QString, int>(*_codeTree);
    _codeTreeSecond = new QHash<QString, int>(*_codeTree);
    _codeTreeThird = new QHash<QString, int>(*_codeTree);
    _codeTreeFourth = new QHash<QString, int>(*_codeTree);
    QHash<QString, int>::iterator i;
    for(i = _codeTreeFirst->begin(); i != _codeTreeFirst->end(); i++)
    {
        i.value() -= 5;
    }

    for(i = _codeTreeSecond->begin(); i != _codeTreeSecond->end(); i++)
    {
        i.value() -= 10;
    }

    for(i = _codeTreeThird->begin(); i != _codeTreeThird->end(); i++)
    {
        i.value() += 5;
    }

    for(i = _codeTreeFourth->begin(); i != _codeTreeFourth->end(); i++)
    {
        i.value() += 10;
    }
}
