#include "quarterimageanalyser.h"

QuarterImageAnalyser::QuarterImageAnalyser(const QString &fileName, const QString &imageTableName) :
    ImageAnalyser(fileName, imageTableName)
{
    _choosenQuarter = 0;
}
