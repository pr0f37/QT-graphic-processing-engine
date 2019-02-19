#ifndef SIFTANALYSER_H
#define SIFTANALYSER_H
#include "analyser.h"
#include "logger.h"
#include "siftdescriptor.h"
#include <sstream>
#include <stdexcept>
#include <string>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#include <QMessageBox>
#include <QLinkedList>
#include <QMutableLinkedListIterator>
#include <QMap>
#include <QMapIterator>
#include <QStringList>
#include <QXmlStreamReader>
#include "siftimage.h"
#include "siftclasscontainer.h"
#include "siftdictionarysections.h"
extern "C" {
    #include "sift.h"
    #include "imgfeatures.h"
    extern int sift_features( IplImage* img, struct feature** feat );
    extern int export_features(char *filename, struct feature *feat, int n);

}

class SiftAnalyser : public Analyser
{
    Q_OBJECT
public:
    QLinkedList<SiftDescriptor*> _siftDescriptors;
    QLinkedList<SiftDescriptor*> _siftDictionary;
    QLinkedList<SiftImage*> _siftImages;
    QMap<int, SiftClassContainer *> _siftContainers; // uzywane do updatowania slownika

    SiftAnalyser(const QString &, const QString &);
    void analyseAndFindClosestImages(const QString&);
    void fillImageSiftDescriptorsList();
    double siftEuclideanDistance(const SiftDescriptor& , const SiftDescriptor &);
    double siftMaximumDistance(const SiftDescriptor& , const SiftDescriptor &);
    void drawInitialDictionaryWords(int);
    void drawImages(int, int);
    void kMeans(QLinkedList<SiftDescriptor*>&, QLinkedList<SiftDescriptor*>&);
    void kMeans(QLinkedList<SiftImage*>&, QLinkedList<SiftDescriptor*>&);
    void findImageClasses(QLinkedList<SiftDescriptor*>&, QLinkedList<SiftDescriptor*>&);
    void findImageClasses(QLinkedList<SiftImage*>&, QLinkedList<SiftDescriptor *>&);
    void determineClosestImageClass(QLinkedList<SiftDescriptor *> &, QLinkedList<SiftDescriptor *> &);
    void determineClosestImageClass(QLinkedList<SiftImage*>&, QLinkedList<SiftDescriptor *>&);
    void getImageSiftBth(QLinkedList<SiftImage*>&, bool = true);
    int getImageSiftBth(SiftImage&, bool = true);
    void getImageHistogram(SiftImage&, bool = true, bool = false);
    bool findNewMeans(QLinkedList<SiftDescriptor *> &, QLinkedList<SiftDescriptor *> &);
    bool findNewMeans(QMap<int, SiftClassContainer *> &, QLinkedList<SiftDescriptor *> &);
    SiftDescriptor * findClassMean(QLinkedList<SiftDescriptor *> &, int);
    SiftDescriptor * findClassMean(QList<SiftDescriptor *> &, int );
    void updateDictionary();
    void hartigansInit();
    SiftDescriptor * meanOfDescriptors();
    QMap<double, SiftDescriptor *> * meanNeighboursSort(SiftDescriptor &);
    void drawInitialDictionaryWords(int, QMap<double, SiftDescriptor *> &);

    void updateConfigFile();
    void analyseImage(const QImage&, const QString&);
    void analyseImage(QImage *);
    void createDictionary();
    SiftImage* createSiftImgFromXML(QXmlStreamReader *);
    SiftDescriptor* createSiftDescriptorFromXML(QXmlStreamReader*);

    void threesholdFilter(QImage *, int);
    void setNewDictionary(bool);
    void setThresholdValue(double);
    void setBlockNumH(int);
    void setBlockNumW(int);
    void setImagesNum(int);
    void setMinValue(int);
    void setMaxValue(int);
    void setVectNum(int);
    void setNumberOfWords(short);
    void setDistance(int);
    void setQueryXBlock(int);
    void setQueryYBlock(int);
    void setQueryXMaxBlock(int);
    void setQueryYMaxBlock(int);
    void setDictionaryInitMethod(int);
    void setOnlyBTH(bool);
    void setDistanceMethod(int);
    void setDivisionMethod(int);
    bool readDictConfFile(const QString &);
    bool readImgConfFile(const QString &);
    void countDistances();
    void countDistancesBTH(SiftImage &, SiftImage&);
    void countDistancesNormalized(SiftImage &, SiftImage&);
    QString findClosestImages(SiftImage &, uint);
    uint * distance(int *, int *);
    double * distance(double *, double *);
    void init();

signals:
    void sendIdValue(const QString);
public:
    static const int _numberOfDesc = 128;
    static const int _maxSiftDescValue = 255;
    short _numberOfWords;
    int _distance;
    int _minValue;
    int _maxValue;
    double _thresholdValue;
    bool _newDictionary;
    int _blockNumH;
    int _blockNumW;
    int _imagesNum;
    int _vectorNum;
    bool _isBlocksConfChanged;
    int _queryXBlock;
    int _queryYBlock;
    int _queryXMaxBlock;
    int _queryYMaxBlock;
    bool _onlyBTH;
    int _dictionaryInitMethod;
    int _distanceMethod;
    int _divisionMethod;
};

#endif // SIFTANALYSER_H
