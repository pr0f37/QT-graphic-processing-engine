#ifndef LSSANALYSER_H
#define LSSANALYSER_H
#include "analyser.h"
#include "logger.h"
#include "cxcore.h"
#include "cv.h"
#include "cvaux.h"
#include "highgui.h"
#include "ml.h"
#include "lssimage.h"
#include "lssclasscontainer.h"
#include <QLinkedList>
#include <QLinkedListIterator>
#include <QXmlStreamReader>
#include <QStringList>
using namespace cv;
class LssAnalyser : public Analyser
{
    Q_OBJECT
public:
    QLinkedList<LssDescriptor *> _dictionary;
    QLinkedList<LssImage *> _images;
    QMap<int, LssClassContainer *> _lssContainers;
    short _dictWordsNum;
    double _thresholdValue;
    uint _distance;


    LssAnalyser(const QString &, const QString &);
    void updateConfigFile();
    void analyseImage(const QImage &, const QString &);
    LssImage analyseImage(const QString &);
    void getBthHistogram(LssImage &);
    int getDescClosestClass(const LssDescriptor&);
    double lssEuclideanDistance(const LssDescriptor&, const LssDescriptor&);
    void createDictionary();
    void updateDictionary();
    void kMeans();
    bool findNewMeans();
    LssDescriptor * findClassMean(QList<LssDescriptor *>&);
    QString findClosestImages(LssImage &, uint);
    void analyseImageAndFindClosestImages(const QString &);
    bool readDictConfFile(const QString &);
    bool readImgConfFile(const QString &);
    LssImage * createLssImgFromXML(QXmlStreamReader *);
    LssDescriptor * createLssDescriptorFromXML(QXmlStreamReader*);

    void countDistances(LssImage &, LssImage&);
    uint * distance(int *, int *);
    void fillImageLssDescriptorsList();

    void setDictWordsNum(short);
    void setThresholdValue(double);
    void setDistance(uint);


signals:
    void sendIdValue(const QString);
    
public slots:

};

#endif // LSSANALYSER_H
