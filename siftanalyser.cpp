#include "siftanalyser.h"

/* PROCEDURY INICJUJACE */
SiftAnalyser::SiftAnalyser(const QString &fileName, const QString &imageTableName):Analyser(fileName, imageTableName)
{
    this->_newDictionary = true;
    this->_thresholdValue = 4;
    this->_blockNumH = 1;
    this->_blockNumW = 1;
    this->_isBlocksConfChanged = true;
    this->_numberOfWords = 200;
    this->_onlyBTH = false;
    this->_dictionaryInitMethod = 0;
    this->_distanceMethod = 0;
    this->_divisionMethod = 0;
}

void SiftAnalyser::analyseImage(const QImage &image, const QString &imageName)
{

}

void SiftAnalyser::updateConfigFile()
{
    QString fileName = _fileName;
    fileName.chop(4);
    fileName += "_" + QString::number(_siftImages.size()) + ".txt";
    QFile * file = new QFile(fileName);
    if(!file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Nie mozna zapisac konfiguracji obrazow."));
        msgBox.exec();
    }
    else
    {
        QTextStream out(file);
        out << "<config>";
        out << "<imagesCount>" << (_siftImages.size()) << "</imagesCount>";
        QLinkedListIterator<SiftImage *> i(_siftImages);
        while (i.hasNext())
        {
            SiftImage * img = i.next();
            QString message = img->toString();
            out << "<image>" << message << "</image>";

        }
        out << "</config>";
        file->close();
    }

    fileName = _fileName;
    fileName.chop(4);
    fileName += "_" + QString::number(_siftImages.size()) +
            "_" + QString::number(_siftDictionary.size()) + "_dict.txt";
    file = new QFile(fileName);

    if(!file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Nie mozna zapisac konfiguracji slownika."));
        msgBox.exec();
    }
    else
    {
        QTextStream out(file);
        out << "<config>";
        out << "<imagesCount>" << (_siftDictionary.size()) << "</imagesCount>";
        QLinkedListIterator<SiftDescriptor *> i(_siftDictionary);
        while (i.hasNext())
        {
            SiftDescriptor * desc = i.next();
            QString message = desc->toString();
            out << "<descriptor>" << message << "</descriptor>";

        }
        out << "</config>";
        file->close();
    }
}

void SiftAnalyser::init()
{
    QSqlQuery query;
    QString queryContent = "SELECT count(*) FROM images";
    query.exec(queryContent);
    QSqlRecord record = query.record();
    query.next();
    int imgNum = query.value(0).toInt();
    QString fileName, fileDictName;
    fileName = fileDictName = _fileName;
    fileName.chop(4);
    fileDictName.chop(4);
    fileName += "_" + QString::number(imgNum) + ".txt";
    fileDictName += "_" + QString::number(imgNum) + "_"
            + QString::number(this->_numberOfWords) + "_dict.txt";
    if(!this->readDictConfFile(fileDictName))
       this->_siftDictionary.clear();

    if(!this->readImgConfFile(fileName))
    {
        this->_siftImages.clear();
        this->_siftDescriptors.clear();
    }
}


bool SiftAnalyser::readDictConfFile(const QString &dictFile)
{
    this->_siftDictionary.clear();
    QFile * file = new QFile(dictFile);
    bool isError = false;
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Nie mozna odczytac pliku konfiguracyjnego slownika obrazow."));
        msgBox.exec();
        return false;
    }
    else
    {
        QXmlStreamReader  * xml = new QXmlStreamReader(file);
        QString tokenAsString;
        while (!xml->atEnd() && !isError)
        {
            QXmlStreamReader::TokenType i = xml->readNext();
            switch(i)
            {
                case QXmlStreamReader::StartElement:
                    tokenAsString = xml->name().toString();
                    if(tokenAsString == "descriptor")
                        this->_siftDictionary << createSiftDescriptorFromXML(xml);
                    break;
                case QXmlStreamReader::Characters:
                    if(tokenAsString == "imagesCount")
                    {
                        int k = xml->text().toString().toInt();
                        (this->_numberOfWords == k)?(isError = false):(isError = true);
                    }
                    break;
                case QXmlStreamReader::EndElement:
                    tokenAsString = xml->name().toString();
                    break;
                case QXmlStreamReader::EndDocument:
                    return true;
                default:
                    break;
            }

        }
        if (xml->hasError() || isError)
        {
            QMessageBox msgBox;
            msgBox.setText(tr("Blad skladni XML pliku konfiguracyjnego bazy obrazow."));
            msgBox.exec();
            return false;
        }
    }
    return true;
}

bool SiftAnalyser::readImgConfFile(const QString &imgFile)
{
    this->_siftImages.clear();
    this->_siftDescriptors.clear();



    QFile * file = new QFile(imgFile);//"SIFT_Images_150.xml");


    if(!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Nie mozna odczytac pliku konfiguracyjnego bazy obrazow."));
        msgBox.exec();
        return false;
    }
    else
    {
        QXmlStreamReader  * xml = new QXmlStreamReader(file);
        QString tokenAsString;
        while (!xml->atEnd())
        {
            QXmlStreamReader::TokenType i = xml->readNext();
            switch(i)
            {
                case QXmlStreamReader::StartElement:
                    tokenAsString = xml->name().toString();
                    if(tokenAsString == "image")
                    {
                        SiftImage * img = createSiftImgFromXML(xml);
                        _siftImages << img;
                    }
                    break;
                case QXmlStreamReader::EndElement:
                    tokenAsString = xml->name().toString();
                    break;
                default:
                    break;
            }

        }
        if (xml->hasError())
        {
            QMessageBox msgBox;
            msgBox.setText(tr("Blad skladni XML pliku konfiguracyjnego bazy obrazow."));
            msgBox.exec();
            return false;
        }
    }

    return true;
}

SiftImage * SiftAnalyser::createSiftImgFromXML(QXmlStreamReader *xml)
{
    QString name;
    int height = 0;
    int width = 0;
    int id = 0;
    int cls = 0;
    bool featuresListEnded = false;
    bool featuresListStarted = false;
    QLinkedList<SiftDescriptor*> featuresList;
    QString tokenAsString;
    QXmlStreamAttributes attributes;
    const QString * text;
    while (!xml->atEnd())
    {
        QXmlStreamReader::TokenType i = xml->readNext();

        switch(i)
        {
            case QXmlStreamReader::StartElement:
                tokenAsString = xml->name().toString();
                attributes = xml->attributes();
                if(tokenAsString == "featuresList" && !featuresListEnded && !featuresListStarted)
                    featuresListStarted = true;
                else if(tokenAsString == "descriptor" && !featuresListEnded && featuresListStarted)
                    featuresList << createSiftDescriptorFromXML(xml);
                break;
            case QXmlStreamReader::EndElement:
                tokenAsString = xml->name().toString();
                if(tokenAsString == "featuresList" && !featuresListEnded && featuresListStarted)
                {
                    featuresListStarted = false;
                    featuresListEnded = true;
                }
                else if(tokenAsString == "image" && featuresListEnded && !featuresListStarted)
                    return new SiftImage(name, featuresList, height, width, id, cls);
                break;
            case QXmlStreamReader::Characters:
                text = xml->text().string();
                if(tokenAsString == "id" && !featuresListStarted)
                    id = text->toInt();
                else if(tokenAsString == "class" && !featuresListStarted)
                    cls = text->toInt();
                else if(tokenAsString == "name" && !featuresListStarted)
                    name = *text;
                else if(tokenAsString == "height" && !featuresListStarted)
                    height = text->toInt();
                else if(tokenAsString == "width"  && !featuresListStarted)
                    width = text->toInt();
                break;
            default:
                break;
        }

    }
    if (xml->hasError())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Blad skladni XML pliku konfiguracyjnego bazy obrazow."));
        msgBox.exec();
    }
    return NULL;
}

SiftDescriptor * SiftAnalyser::createSiftDescriptorFromXML(QXmlStreamReader *xml)
{
    QString name;
    int clas = 0;
    double x = 0;
    double y = 0;
    double a = 0;
    double b = 0;
    double c = 0;
    double scl = 0;
    double ori = 0;
    int * descriptor = new int[SiftDescriptor::_numberOfDesc];
    QString tokenAsString;
    QXmlStreamAttributes attributes;
    const QString * text;
    while (!xml->atEnd())
    {
        QXmlStreamReader::TokenType i = xml->readNext();

        switch(i)
        {
            case QXmlStreamReader::StartElement:
                tokenAsString = xml->name().toString();
                attributes = xml->attributes();
                break;
            case QXmlStreamReader::EndElement:
                tokenAsString = xml->name().toString();
                if (tokenAsString == "descriptor")
                    return new SiftDescriptor(name, descriptor, clas, x, y, a, b, c, scl, ori);
                break;
            case QXmlStreamReader::Characters:
                text = xml->text().string();
                if (tokenAsString == "class")
                    clas = text->toInt();
                else if (tokenAsString == "name")
                    name = *text;
                else if (tokenAsString == "x")
                    x = text->toDouble();
                else if (tokenAsString == "y")
                    y = text->toDouble();
                else if (tokenAsString == "a")
                    a = text->toDouble();
                else if (tokenAsString == "b")
                    b = text->toDouble();
                else if (tokenAsString == "c")
                    c = text->toDouble();
                else if (tokenAsString == "scl")
                    scl = text->toDouble();
                else if (tokenAsString == "ori")
                    ori = text->toDouble();
                else if (tokenAsString == "_descriptor")
                {
                    QStringList sl = text->split(";", QString::SkipEmptyParts);
                    QStringListIterator si(sl);
                    int i = 0;
                    while (si.hasNext())
                        *(descriptor+i++) = si.next().toInt();
                }
                break;
            default:
                break;
        }
    }
    if (xml->hasError())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Blad skladni XML pliku konfiguracyjnego bazy obrazow."));
        msgBox.exec();

    }
    return NULL;
}

        /* PROCEDURY PRZETWARZAJACE OBRAZY */
/* losowany slownik bez uzycia kmeans */
void SiftAnalyser::analyseImage(QImage *image)
{
    Logger * log_i = new Logger("SIFT_BHT_Images.csv");
    if(_siftImages.isEmpty())
        this->fillImageSiftDescriptorsList();
    if (_newDictionary || _siftDictionary.isEmpty())
        this->drawInitialDictionaryWords(this->_numberOfWords);
    this->findImageClasses(_siftImages, _siftDictionary);
    this->getImageSiftBth(_siftImages);
    log_i->logSimple(_siftImages, this->_numberOfWords);

}
/* Slownik losowany a potem dopasowany do danych za pomoca kmeans*/
void SiftAnalyser::createDictionary()
{
    Logger * log_s = new Logger("SIFT_HST_Images_" + QString::number(this->_numberOfWords) + "_" + QString::number(this->_thresholdValue) + ".csv");
    Logger * log_b = new Logger("SIFT_BHT_Images_" + QString::number(this->_numberOfWords) + "_" + QString::number(this->_thresholdValue) + ".csv");
    Logger * log_n = new Logger("SIFT_HSN_Images_" + QString::number(this->_numberOfWords) + "_" + QString::number(this->_thresholdValue) + ".csv");
    if (_siftImages.isEmpty())
        this->fillImageSiftDescriptorsList();

    if (this->_newDictionary || this->_siftDictionary.isEmpty())
    {
        if (this->_dictionaryInitMethod == 0) // mine
        {
            this->drawInitialDictionaryWords(this->_numberOfWords);
            this->determineClosestImageClass(_siftImages, _siftDictionary); // przypisanie wartosci do wylosowanych slow slownika
            this->updateDictionary(); // znalezienie najczesciej wystepujacych slow w slowniku, wylosowanie nowych slow sposrod przedstawicieli najliczniejszych klas
        }
        else
            this->hartigansInit();

        this->kMeans(_siftImages, _siftDictionary);
    }

    this->getImageSiftBth(_siftImages);

    this->updateConfigFile();

    log_s->logSimple(this->_siftImages, this->_numberOfWords, 1);
    log_b->logSimple(this->_siftImages, this->_numberOfWords, 2);
    log_n->logSimple(this->_siftImages, this->_numberOfWords, 3);
}

void SiftAnalyser::drawAndAnalyseImage(QImage *image)
{
    Logger * log_i = new Logger("SIFT_BHT_Images.csv");
    if (_newDictionary || _siftDictionary.isEmpty())
        this->drawInitialDictionaryWords(this->_numberOfWords);
    this->drawImages(_imagesNum, _vectorNum);
    this->findImageClasses(_siftImages, _siftDictionary);
    this->getImageSiftBth(_siftImages, false);
    log_i->logSimple(_siftImages, this->_numberOfWords);
}

void SiftAnalyser::analyseAndFindClosestImages(const QString& fileName)
{
    Logger *log = new Logger("findClosestImagesSIFT.txt");
    if (this->_siftImages.isEmpty() || this->_newDictionary || this->_siftDictionary.isEmpty())
        this->createDictionary();
    if(this->_onlyBTH)
        this->getImageSiftBth(_siftImages);

    QByteArray byteArray = fileName.toUtf8();
    const char * file = byteArray.constData();
    IplImage* img;

    img = cvLoadImage(file, 1);
    if (!img)
        log->logSimple("Error while opening file: " + QString::fromUtf8(file) + "\n");
    else
    {

        struct feature * features ;
        int n = sift_features(img, &features);
        SiftImage * analysedImage = new SiftImage(fileName, features, n, img->width, img->height);
        this->determineClosestImageClass(analysedImage->_featuresList, this->_siftDictionary);
        if (_isBlocksConfChanged)
        {
            this->getImageSiftBth(_siftImages, true);
            _isBlocksConfChanged = false;
        }
        this->getImageHistogram(*(analysedImage), true, true);
        QSqlQuery query;
        QString queryContent = "update images set bst_key ='999999999'";
        query.exec(queryContent);
        Logger * log_t = new Logger("Actions_Times_" + QString::number(this->_numberOfWords) + "_"  + QString::number(this->_thresholdValue) + ".csv");
        log_t->logTimeAndAction("Start");
        QString list = findClosestImages(*(analysedImage), this->_distance);
        log_t->logTimeAndAction("Porownanie obrazow");
        for (int n = 5; n < 26; n += 5)
        {
            QLinkedList<SiftImage*> tmpList;
            tmpList << analysedImage;
            QString name = analysedImage->_name.remove(QRegExp("(.*(?=/))*")).remove(QRegExp("/"));
            name.chop(4);
            Logger * log_d = new Logger("Distances_" + name + "_" + QString::number(this->_numberOfWords) + "_" + QString::number(n) + "_" + QString::number(this->_thresholdValue) + ".csv");
            log_d->logDistance(tmpList, n);

        }

        emit sendIdValue(list);
        delete features;
    }
    cvReleaseImage(&img);
    delete img;
}

void SiftAnalyser::fillImageSiftDescriptorsList()
{
    this->_siftDescriptors.clear();
    this->_siftImages.clear();
    QSqlRelationalTableModel *imageTable = new QSqlRelationalTableModel(this);
    Logger * log = new Logger("SiftImageLog.txt");
    int n;
    imageTable->setTable(_imageTableName);
    imageTable->select(); // reads image table from database
    int imageCount = imageTable->rowCount();
    for (int i = 0; i <imageCount; i++)
    {
        QSqlRecord record = imageTable->record(i); // reads one record from image table
        QString fileQString = record.value("file").toString();
        int fileId = record.value("id").toInt();
        int cls = record.value("cls").toInt();
        QByteArray byteArray = fileQString.toUtf8();
        const char * file = byteArray.constData();
        IplImage* img;

        img = cvLoadImage(file, 1);
        if (!img)
            log->logSimple("Error while opening file: " + QString::fromUtf8(file) + "\n");
        else
        {
            struct feature* features ;
            n = sift_features(img, &features);

            for (int j = 0; j < n; j++ )
               _siftDescriptors <<  (new SiftDescriptor(fileQString, (features+j)->descr));

            _siftImages << (new SiftImage(fileQString, features, n, img->width, img->height, fileId, cls));
            delete features;
        }
        cvReleaseImage(&img);
        delete img;
    }
}

double SiftAnalyser::siftEuclideanDistance(const SiftDescriptor& firstSD , const SiftDescriptor & secondSD)
{
    double distance = 0.0;
    for (int i = 0; i < SiftDescriptor::_numberOfDesc; i++)
    {
        int di = *(firstSD._descriptor + i) - *(secondSD._descriptor + i);
        distance += di*di;
    }
    distance = sqrt(distance);
    return distance;
}

double SiftAnalyser::siftMaximumDistance(const SiftDescriptor& firstSD , const SiftDescriptor & secondSD)
{
    double distance = 0.0;
    for (int i = 0; i < SiftDescriptor::_numberOfDesc; i++)
    {
        int di = abs(*(firstSD._descriptor + i) - *(secondSD._descriptor + i));
        if (di > distance)
            distance = di;
    }

    return distance;
}

void SiftAnalyser::kMeans(QLinkedList<SiftDescriptor *> &siftDescriptors, QLinkedList<SiftDescriptor *> &dictionary)
{
    Logger * log_d = new Logger("SIFT_Dictionary_changes.csv");
    Logger * log_i = new Logger("SIFT_Images_changes.csv");
    Logger * log_l = new Logger("SIFT_loops.txt");
    int i = 0;
    bool meanChanged = true;
    while (meanChanged == true)
    {
        log_l->log(QString::number(++i));
        log_i->logSimple(siftDescriptors);
        log_d->logSimple(dictionary);
        determineClosestImageClass(siftDescriptors, dictionary);
        meanChanged = findNewMeans(siftDescriptors, dictionary);
    }
}

void SiftAnalyser::kMeans(QLinkedList<SiftImage*> &siftImages, QLinkedList<SiftDescriptor *> &dictionary)
{
//    Logger * log_d = new Logger("SIFT_Dictionary_changes.csv");
//    Logger * log_i = new Logger("SIFT_Images_changes.csv");
//    Logger * log_l = new Logger("SIFT_loops.txt");
    int i = 0;
    bool meanChanged = true;
    while (meanChanged == true && i < 10)
    {
//        log_l->log(QString::number(++i));
//        log_i->logSimple(siftImages, this->_numberOfWords);
//        log_d->logSimple(dictionary);
        this->determineClosestImageClass(siftImages, dictionary);
        meanChanged = findNewMeans(_siftContainers, dictionary);

    }
}

void SiftAnalyser::findImageClasses(QLinkedList<SiftDescriptor *> &siftDescriptors, QLinkedList<SiftDescriptor *> &dictionary)
{
    Logger * log_d = new Logger("SIFT_Dictionary_changes_" + QString::number(this->_numberOfWords) + "_" + QString::number(this->_thresholdValue) + ".csv");
    Logger * log_i = new Logger("SIFT_Images_changes_"+ QString::number(this->_numberOfWords) + "_" + QString::number(this->_thresholdValue) + ".csv");
    determineClosestImageClass(siftDescriptors, dictionary);
    log_i->logSimple(siftDescriptors);
    log_d->logSimple(dictionary);
}

void SiftAnalyser::findImageClasses(QLinkedList<SiftImage*> &siftImages, QLinkedList<SiftDescriptor *> &dictionary)
{
    Logger * log_i = new Logger("SIFT_Images_changes_" + QString::number(this->_numberOfWords) + "_" + QString::number(this->_thresholdValue) + ".csv");
    Logger * log_d = new Logger("SIFT_Dictionary_changes_"+ QString::number(this->_numberOfWords) + "_" + QString::number(this->_thresholdValue) + ".csv");
    determineClosestImageClass(siftImages, dictionary);
    log_i->logSimpleSIFT(siftImages);
    log_d->logSimple(dictionary);

}

void SiftAnalyser::determineClosestImageClass(QLinkedList<SiftDescriptor *> &examples, QLinkedList<SiftDescriptor *> &dictionary)
{
    QMutableLinkedListIterator<SiftDescriptor *> ei(examples);
    while (ei.hasNext())
    {
        SiftDescriptor * example = ei.next();
        double dist = std::numeric_limits<double>::max();
        QMutableLinkedListIterator<SiftDescriptor *> di(dictionary);
        while (di.hasNext())
        {
            SiftDescriptor * dict = di.next();
            //double newDist = this->siftMaximumDistance(*example, *dict);
            double newDist = this->siftEuclideanDistance(*example, *dict);
            if(newDist <= dist)
            {
                dist = newDist;
                example->_class = dict->_class;
            }
        }           
        if (!_siftContainers.contains(example->_class))
            _siftContainers.insert(example->_class, new SiftClassContainer(example->_class));
        _siftContainers[example->_class]->addSiftToContainer(*example);
    }

}

void SiftAnalyser::determineClosestImageClass(QLinkedList<SiftImage*> &siftImage, QLinkedList<SiftDescriptor *> &dictionary)
{
    _siftContainers.clear();
    QMutableLinkedListIterator<SiftImage *> ei(siftImage);
    while (ei.hasNext())
    {
        SiftImage * img = ei.next();
        this->determineClosestImageClass(img->_featuresList, dictionary);
    }
}

void SiftAnalyser::getImageSiftBth(QLinkedList<SiftImage *> &siftImages, bool blocks)
{
    QMutableLinkedListIterator<SiftImage *> ei(siftImages);
    while (ei.hasNext())
    {
        SiftImage * img = ei.next();
        this->getImageHistogram(*img, blocks);
    }
}

int SiftAnalyser::getImageSiftBth(SiftImage &image, bool blocks)
{
    this->getImageHistogram(image, blocks);
    return 0;
    // zaproponowac bth jako 6 liczb uint
}

void SiftAnalyser::getImageHistogram(SiftImage &image, bool blocks, bool isQuery)
{
    while (!image._siftHistogramN.isEmpty())
        delete image._siftHistogramN.takeFirst();
    while (!image._siftHistogram.isEmpty())
        delete image._siftHistogram.takeFirst();
    while (!image._siftBth.isEmpty())
        delete image._siftBth.takeFirst();
    image._siftHistogramN.clear();
    image._siftHistogram.clear();
    image._siftBth.clear();


    if (this->_divisionMethod == 0)
    {
        if (!blocks)
        {
            this->_blockNumH = 1;
            this->_blockNumW = 1;
        }
        int blockH = image._height / this->_blockNumH; // jesli blocks jest false, dziele przez 1
        int blockW = image._width / this->_blockNumW;

        for (int y = (isQuery?this->_queryYBlock:0); y < (isQuery?(this->_queryYMaxBlock + 1):this->_blockNumH); y++)
            for (int x = (isQuery?this->_queryXBlock:0); x < (isQuery?(this->_queryXMaxBlock + 1):this->_blockNumW); x++)
            {
                int siftDescNum = 0;
                int * imageClassHistogram = new int[this->_numberOfWords];
                int * imageClassHistogramBTH = new int[this->_numberOfWords];
                double * imageClassHistogramNormalised = new double[this->_numberOfWords];
                for (int i = 0; i < this->_numberOfWords; i++)
                {
                    *(imageClassHistogram + i) = 0;
                    *(imageClassHistogramBTH + i) = 0;
                }
                QMutableLinkedListIterator<SiftDescriptor *> fi(image._featuresList);
                while (fi.hasNext())
                {
                    SiftDescriptor * sd = fi.next();
                    if(sd->_x >= x*blockW && sd->_x < (x+1)*blockW
                            &&
                            sd->_y >= y*blockH && sd->_y < (y+1)*blockH)
                    {
                        *(imageClassHistogram + sd->_class) += 1;
                        siftDescNum++;
                    }
                }
                image._siftHistogram << imageClassHistogram;
                if (siftDescNum != 0)
                    for (int i = 0; i < this->_numberOfWords; i++)
                    {
                        *(imageClassHistogramNormalised + i) = (double)(((double)*(imageClassHistogram + i))/((double)siftDescNum));
                        if (*(imageClassHistogramNormalised + i) * 100 >= this->_thresholdValue)
                            *(imageClassHistogramBTH + i) = 1;
                        else
                            *(imageClassHistogramBTH + i) = 0;
                    }
                image._siftHistogramN << imageClassHistogramNormalised;
                image._siftBth << imageClassHistogramBTH;
            }
    }
    else if (this->_divisionMethod == 1) // Pyramid representation
    {
        for (int level = 0; level < 3; level++)
        {
            int blocks = (int)pow(2,level);
            int blockH = image._height / blocks;
            int blockW = image._width / blocks;
            double beta = (level == 0)?(1 / pow(2,3 - 1)):(1 / pow(2,3 - level));
            for (int y = 0; y < blocks; y++)
                for (int x = 0; x < blocks; x++)
                {
                    int siftDescNum = 0;
                    int * imageClassHistogram = new int[this->_numberOfWords];
                    int * imageClassHistogramBTH = new int[this->_numberOfWords];
                    double * imageClassHistogramNormalised = new double[this->_numberOfWords];
                    for (int i = 0; i < this->_numberOfWords; i++)
                    {
                        *(imageClassHistogram + i) = 0;
                        *(imageClassHistogramBTH + i) = 0;
                        *(imageClassHistogramNormalised + i) = 0;
                    }
                    QMutableLinkedListIterator<SiftDescriptor *> fi(image._featuresList);
                    while (fi.hasNext())
                    {
                        SiftDescriptor * sd = fi.next();
                        if(sd->_x >= x*blockW && sd->_x < (x+1)*blockW
                                &&
                                sd->_y >= y*blockH && sd->_y < (y+1)*blockH)
                        {
                            *(imageClassHistogram + sd->_class) += 1;
                            siftDescNum++;
                        }
                    }
                    image._siftHistogram << imageClassHistogram;
                    if (siftDescNum != 0)
                        for (int i = 0; i < this->_numberOfWords; i++)
                        {
                            *(imageClassHistogramNormalised + i) = ((double)(((double)*(imageClassHistogram + i))/((double)siftDescNum)));

//                            *(imageClassHistogramNormalised + i) /= (double)(blocks*blocks);
                            *(imageClassHistogramNormalised + i) *= beta;
                            if (*(imageClassHistogramNormalised + i) * 100 >= this->_thresholdValue)
                                *(imageClassHistogramBTH + i) = 1;
                            else
                                *(imageClassHistogramBTH + i) = 0;
                        }
                    image._siftHistogramN << imageClassHistogramNormalised;
                    image._siftBth << imageClassHistogramBTH;
                }
        }
    }

}

bool SiftAnalyser::findNewMeans(QLinkedList<SiftDescriptor *> &examples, QLinkedList<SiftDescriptor *> &dictionary)
{
    bool meanChanged = false;
    QMutableLinkedListIterator<SiftDescriptor *> di(dictionary);
    while (di.hasNext())
    {
        SiftDescriptor * dict = di.next();
        SiftDescriptor * newMean = findClassMean(examples, dict->_class);
        if (newMean != 0)
        {
            double dist = this->siftEuclideanDistance(*dict, *newMean);
            if (dist != 0)
            {
                meanChanged = true;
                dict->_descriptor = newMean->_descriptor;
            }
        }
    }
    return meanChanged;
}

bool SiftAnalyser::findNewMeans(QMap<int, SiftClassContainer* > &containers, QLinkedList<SiftDescriptor *> &dictionary)
{
    bool meanChanged = false;
    QMutableLinkedListIterator<SiftDescriptor *> di(dictionary);
    while (di.hasNext())
    {
        SiftDescriptor * dict = di.next();
        if (containers.contains(dict->_class))
        {
            QList<SiftDescriptor *> myList = containers[dict->_class]->_descriptors.values();
            SiftDescriptor * newMean = findClassMean(myList, dict->_class);
            if (newMean != 0)
            {
                double dist = this->siftEuclideanDistance(*dict, *newMean);
                if (dist != 0)
                {
                    meanChanged = true;
                    dict->_descriptor = newMean->_descriptor;
                }
            }
        }
    }
    return meanChanged;
}

SiftDescriptor * SiftAnalyser::findClassMean(QLinkedList<SiftDescriptor *> &examples, int cls)
{

    int clsReps = 0;
    QMutableLinkedListIterator<SiftDescriptor *> ei(examples);
    int * descriptor = new int[SiftDescriptor::_numberOfDesc];
    for (int i = 0; i < SiftDescriptor::_numberOfDesc; i++)
        *(descriptor + i) = 0;
    while (ei.hasNext())
    {
        SiftDescriptor * example = ei.next();
        if(example->_class == cls)
        {
            clsReps++;
            for (int i = 0; i < SiftDescriptor::_numberOfDesc; i++)
                *(descriptor + i) += *(example->_descriptor + i);
        }

    }
    if (clsReps != 0)
    {
        for (int i = 0; i < SiftDescriptor::_numberOfDesc; i++)
            *(descriptor + i) = static_cast<int>(*(descriptor + i)/clsReps); // zaokraglamy do calosci bo sift powinno byc intami
        SiftDescriptor * newMeanDesc = new SiftDescriptor("newMean", descriptor, cls);
        return newMeanDesc;
    }
    return 0;
}

SiftDescriptor * SiftAnalyser::findClassMean(QList<SiftDescriptor *> &examples, int cls)
{

    int clsReps = 0;
    QMutableListIterator<SiftDescriptor *> ei(examples);
    int * descriptor = new int[SiftDescriptor::_numberOfDesc];
    for (int i = 0; i < SiftDescriptor::_numberOfDesc; i++)
        *(descriptor + i) = 0;
    while (ei.hasNext())
    {
        SiftDescriptor * example = ei.next();
        if(example->_class == cls)
        {
            clsReps++;
            for (int i = 0; i < SiftDescriptor::_numberOfDesc; i++)
                *(descriptor + i) += *(example->_descriptor + i);
        }

    }
    if (clsReps != 0)
    {
        for (int i = 0; i < SiftDescriptor::_numberOfDesc; i++)
            *(descriptor + i) = static_cast<int>(*(descriptor + i)/clsReps); // zaokraglamy do calosci bo sift powinno byc intami
        SiftDescriptor * newMeanDesc = new SiftDescriptor("newMean", descriptor, cls);
        return newMeanDesc;
    }
    return 0;
}

void SiftAnalyser::updateDictionary()
{
    _siftDictionary.clear();
    QMapIterator<int, SiftClassContainer *> ci(_siftContainers);
    double powerSum = 0;
    while (ci.hasNext())
    {
        SiftClassContainer * container = ci.next().value();
        powerSum += container->_power;
    }
    ci.toFront();
    while (ci.hasNext())
    {
        SiftClassContainer * container = ci.next().value();
        for (int i = 0; i < qRound((double)this->_numberOfWords * (container->_power / powerSum)) && this->_siftDictionary.size() < this->_numberOfWords; i++)
        {
            int randValue = 0;
            do
                randValue =  qrand() % container->_power;
            while(!container->_descriptors.contains(randValue));
            SiftDescriptor * newDictClass = new SiftDescriptor(*(container->_descriptors.take(randValue))); // wyciagam z kontenera wylosowana wartosc i jej KOPIE umieszczam w slowniku
            newDictClass->_class = _siftDictionary.count();
            _siftDictionary << newDictClass;
        }
    }
    for (int i = this->_siftDictionary.size(); i < this->_numberOfWords; i++)
    {
        int * descriptorValues = new int[SiftDescriptor::_numberOfDesc];

        for (int j = 0; j < SiftDescriptor::_numberOfDesc; j ++)
        {
            int randValue = qrand() % (SiftAnalyser::_maxSiftDescValue + 1);
            *(descriptorValues + j) = randValue;
        }
        SiftDescriptor * newClass = new SiftDescriptor("Representative of the class " + QString::number(i), descriptorValues, i);
        this->_siftDictionary << newClass;
    }
}

void SiftAnalyser::hartigansInit()
{
    SiftDescriptor * descMean = meanOfDescriptors();
    QMap<double, SiftDescriptor*> * meanNeighbours = meanNeighboursSort(*descMean);
    drawInitialDictionaryWords(this->_numberOfWords, *meanNeighbours);

    delete descMean;
    delete meanNeighbours;
}

SiftDescriptor * SiftAnalyser::meanOfDescriptors()
{
    ulong * lDesc = new ulong[SiftDescriptor::_numberOfDesc];
    for (int i = 0; i < SiftDescriptor::_numberOfDesc; i++)
        *(lDesc + i) = 0;
    QLinkedListIterator<SiftDescriptor *> iter(this->_siftDescriptors);
    while(iter.hasNext())
    {
        int * iterDesc = iter.next()->_descriptor;
        for (int i = 0; i < SiftDescriptor::_numberOfDesc; i++)
            *(lDesc + i) += *(iterDesc + i);
    }
    int * iDesc = new int[SiftDescriptor::_numberOfDesc];
    for (int i = 0; i < SiftDescriptor::_numberOfDesc; i++)
        *(iDesc + i) = static_cast<int>(*(lDesc + i) / this->_siftDescriptors.size());
    delete lDesc;
    return new SiftDescriptor("Mean of all descriptors", iDesc);
}

QMap<double, SiftDescriptor*> * SiftAnalyser::meanNeighboursSort(SiftDescriptor & mean)
{
    QMap<double, SiftDescriptor*> * neighbours = new QMap<double, SiftDescriptor*>();
    QLinkedListIterator<SiftDescriptor *> iter(this->_siftDescriptors);
    while(iter.hasNext())
    {
        SiftDescriptor * iterDesc = iter.next();
        double dist = siftEuclideanDistance(mean, *iterDesc);
        neighbours->insertMulti(dist, iterDesc);
    }
    return neighbours;
}

void SiftAnalyser::drawInitialDictionaryWords(int wordsNumber, QMap<double, SiftDescriptor *> & neigbours)
{
    this->_siftDictionary.clear();
    QList<SiftDescriptor*> neighSortedList = neigbours.values();
    for (int i = 0; i < wordsNumber; i++)
    {
        int index = 1 + i*(neighSortedList.count()/wordsNumber);
        int * descriptorValues = new int[SiftDescriptor::_numberOfDesc];

        for (int j = 0; j < SiftDescriptor::_numberOfDesc; j ++)
        {
            *(descriptorValues + j) = *(neighSortedList[index]->_descriptor + j) ;
        }
        SiftDescriptor * newClass = new SiftDescriptor("Representative of the class " + QString::number(i), descriptorValues, i);
        this->_siftDictionary << newClass;
    }
}


void SiftAnalyser::createSiftSections(QMap<int, SiftDictionarySections *> &sections, QLinkedList<SiftImage*> &images, QLinkedList<int> &usedWords)
{
    sections.clear();
    QMutableLinkedListIterator<int> usedWord(usedWords);
    while (usedWord.hasNext())
    {
        SiftDictionarySections * sds = new SiftDictionarySections(usedWord.next());
        sections.insert(sds->_class, sds);
    }

    QMutableLinkedListIterator<SiftImage*> image(images);
    while(image.hasNext())
    {
        SiftImage * i = image.next();
        QMutableLinkedListIterator<SiftDescriptor*> imageFeat(i->_featuresList);
        while (imageFeat.hasNext())
        {
            SiftDescriptor * feat = imageFeat.next();
            sections[feat->_class]->changeSection(*feat);
        }

    }

}

void SiftAnalyser::drawInitialDictionaryWords(int wordsNumber)
{
    this->_siftDictionary.clear();
    uint seed = (uint)QDateTime::currentDateTime().toTime_t();
    qsrand(seed);
    for (int i = 0; i < wordsNumber; i++)
    {
        int * descriptorValues = new int[SiftDescriptor::_numberOfDesc];

        for (int j = 0; j < SiftDescriptor::_numberOfDesc; j ++)
        {
            int randValue = qrand() % (SiftAnalyser::_maxSiftDescValue + 1);
            *(descriptorValues + j) = randValue;
        }
        SiftDescriptor * newClass = new SiftDescriptor("Representative of the class " + QString::number(i), descriptorValues, i);
        this->_siftDictionary << newClass;
    }
}

void SiftAnalyser::drawImages(int imgNumber, int vectNumber)
{
    this->_siftImages.clear();
    for (int k = 0; k < imgNumber; k++)
    {
        QLinkedList<SiftDescriptor *> SiftList;
        for (int i = 0; i < vectNumber; i++)
        {
            int * descriptorValues = new int[SiftDescriptor::_numberOfDesc];

            for (int j = 0; j < SiftDescriptor::_numberOfDesc; j ++)
            {
                int randValue = ( _minValue + qrand()) % (_maxValue + 1);
                *(descriptorValues + j) = randValue;
            }
            SiftDescriptor * newClass = new SiftDescriptor("Random SIFT " + QString::number(i + 1), descriptorValues, i+1);
            SiftList << newClass;
        }
        SiftImage * newImage = new SiftImage("Random Image", SiftList, 1, 1);
        this->_siftImages << newImage;
    }
}


void SiftAnalyser::countDistances()
{
    QMutableLinkedListIterator<SiftImage *> imageIt(this->_siftImages);
    while(imageIt.hasNext())
    {
        SiftImage * image = imageIt.next();
        image->_distances.clear();
        image->_distancesN.clear();
        QMutableLinkedListIterator<SiftImage *> imageIt2(this->_siftImages);
        while(imageIt2.hasNext())
        {
            SiftImage * image2 = imageIt2.next();
            if (image != image2)
            {
                if (_distanceMethod == 0)
                    countDistancesBTH(*image, *image2);
                else if (_distanceMethod == 1)
                    countDistancesNormalized(*image, *image2);
            }
        }
    }
}

/* distance is counted as a sum off all subblocks ALWAYS!!!! */
void SiftAnalyser::countDistancesBTH(SiftImage &imageAnalysed, SiftImage &image)
{
    QMutableLinkedListIterator<int *> analysedIt(imageAnalysed._siftBth);
    QMutableLinkedListIterator<int *> imageIt(image._siftBth);

    uint dist = std::numeric_limits<uint>::max();
    uint tmpDist;
    for (int ys = 0; ys < (this->_blockNumH - (this->_queryYMaxBlock - this->_queryYBlock)); ys++)
        for (int xs = 0; xs < (this->_blockNumW - (this->_queryXMaxBlock - this->_queryXBlock)); xs++)
        {
            analysedIt.toFront();
            imageIt.toFront();
            tmpDist = 0;
            for (int y = 0; y < this->_blockNumH; y++)
                for (int x = 0; x < this->_blockNumW; x++)
                    if (x >= xs && x <= (xs + (this->_queryXMaxBlock - this->_queryXBlock)) && y >= ys && y <= (ys + (this->_queryYMaxBlock - this->_queryYBlock)))
                        tmpDist += *(distance(analysedIt.next(), imageIt.next()));
                    else
                        imageIt.next();
            if (tmpDist < dist)
                dist = tmpDist;
        }


    imageAnalysed._distances.insertMulti(dist, image._id);
}

void SiftAnalyser::countDistancesNormalized(SiftImage &imageAnalysed, SiftImage &image)
{
    QMutableLinkedListIterator<double *> analysedIt(imageAnalysed._siftHistogramN);
    QMutableLinkedListIterator<double *> imageIt(image._siftHistogramN);
    double dist = 0.0;
    uint minVal = 1000;
    uint nDist = 0;
    while(analysedIt.hasNext() && imageIt.hasNext())
    {
        dist += *(distance(analysedIt.next(), imageIt.next()));
        nDist += (uint)minVal*(1 - dist);
    }

    imageAnalysed._distancesN.insertMulti(dist, image._id);
    imageAnalysed._distances.insertMulti(nDist, image._id); // in different order cause bigger dist value means more similiar image in this case
}

QString SiftAnalyser::findClosestImages(SiftImage &imageAnalysed, uint imageNumber)
{
    imageAnalysed._distances.clear();
    imageAnalysed._distancesN.clear();
    QSqlQuery query;
    QStringList * closestImages = new QStringList();
    QMutableLinkedListIterator<SiftImage *> imagesIt(this->_siftImages);
    while(imagesIt.hasNext())
    {
        SiftImage & image = *(imagesIt.next());
        this->countDistancesBTH(imageAnalysed, image);
    }
    QMutableMapIterator<uint, int> distIt(imageAnalysed._distances);
    while(distIt.hasNext() && imageNumber-- > 0)
    {
        QMap<uint, int>::iterator it = distIt.next();
        uint key = it.key();
        int id = it.value();
        closestImages->append(QString::number(id));
        QString queryContent = "update images set bst_key ='" + QString::number(key)
                + "' where id = " + QString::number(id) + "";
        query.exec(queryContent);
    }

    return closestImages->join(",");
}

uint * SiftAnalyser::distance(int * image1Bth, int * image2Bth)
{
    uint * dist = new uint;

    *(dist) = 0;
    for (int i = 0; i < this->_numberOfWords; i++)
        *(dist) += (*(image1Bth+i) ^ *(image2Bth+i));

    return dist;
}

double * SiftAnalyser::distance(double * histNorm1, double * histNorm2)
{
    double * dist = new double;

    *(dist) = 0.0;
    for (int i = 0; i < this->_numberOfWords; i++)
        *(dist) += (*(histNorm1+i)<*(histNorm2+i))?(*(histNorm1+i)):(*(histNorm2+i));

    return dist;

}



void SiftAnalyser::setNewDictionary(bool newDictionary)
{
    this->_newDictionary = newDictionary;
}

void SiftAnalyser::setThresholdValue(double thresholdValue)
{
    this->_thresholdValue = thresholdValue;
}

void SiftAnalyser::setBlockNumH(int blockNumH)
{
    if(blockNumH != _blockNumH)
        _isBlocksConfChanged = true;
    this->_blockNumH = blockNumH;
}

void SiftAnalyser::setBlockNumW(int blockNumW)
{
    if(blockNumW != _blockNumW)
        _isBlocksConfChanged = true;
    this->_blockNumW = blockNumW;
}

void SiftAnalyser::setImagesNum(int imagesNum)
{
    this->_imagesNum = imagesNum;
}

void SiftAnalyser::setMinValue(int value)
{
    this->_minValue = value;
}

void SiftAnalyser::setMaxValue(int value)
{
    this->_maxValue = value;
}

void SiftAnalyser::setVectNum(int value)
{
    this->_vectorNum = value;
}

void SiftAnalyser::setNumberOfWords(short value)
{
    this->_numberOfWords = value;
}

void SiftAnalyser::setDistance(int value)
{
    this->_distance = value;
}

void SiftAnalyser::setQueryXBlock(int value)
{
    this->_queryXBlock = value;
}

void SiftAnalyser::setQueryYBlock(int value)
{
    this->_queryYBlock = value;
}

void SiftAnalyser::setQueryXMaxBlock(int value)
{
    this->_queryXMaxBlock = value;
}

void SiftAnalyser::setQueryYMaxBlock(int value)
{
    this->_queryYMaxBlock = value;
}

void SiftAnalyser::setOnlyBTH(bool value)
{
    _onlyBTH = value;
}

void SiftAnalyser::setDistanceMethod(int distanceMethod)
{
    _distanceMethod = distanceMethod;
}

void SiftAnalyser::setDivisionMethod(int divisionMethod)
{
    _divisionMethod = divisionMethod;
}

void SiftAnalyser::setDictionaryInitMethod(int dictionaryInitMethod)
{
    _dictionaryInitMethod = dictionaryInitMethod;
}
