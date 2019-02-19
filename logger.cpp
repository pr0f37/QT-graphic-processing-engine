#include "logger.h"
#include "siftanalyser.h"
#define doLog 1
Logger::Logger(const QString &logFileName)
{
    _timer = new QTime();
    _logFile = new QFile("./logs/" + logFileName);
    _isTimerOn = false;
    clearLogFile();
}

Logger::~Logger()
{
    delete _timer;
    delete _logFile;
}

bool Logger::logTimeAndAction(const QString &action)
{
    if(doLog)
    {
        if(!_logFile->open(QIODevice::Append | QIODevice::Text))
        {
           QMessageBox msgBox;
           msgBox.setText(tr("Nie mozna otworzyc pliku logowania!"));
           msgBox.exec();
           return false;
        }
        else
        {
            QTextStream out(_logFile);
            if (this->_isTimerOn == false)
            {
                out << "Czynnosc,Czas trwania[ms]\n" << action << ",0\n";
                this->_isTimerOn = true;
                this->_timer->start();
            }
            else
                out << action << "," << QString::number(this->_timer->restart()) << "\n";
            _logFile->close();
        }
    }
    return true;
}

bool Logger::log(const QString &logValue)
{
    if(doLog)
    {
        if(!_logFile->open(QIODevice::Append | QIODevice::Text))
        {
           QMessageBox msgBox;
           msgBox.setText(tr("Nie mozna otworzyc pliku logowania!"));
           msgBox.exec();
           return false;
        }
        else
        {
            QTextStream out(_logFile);
            out << "[LOG TIME :" << (QDateTime::currentDateTime()).toString("hh:mm:ss") << "]: " << logValue << "\n";
            _logFile->close();
        }
    }
    return true;
}

bool Logger::logSimple(const QString &logValue)
{
    if(doLog)
    {
        if(!_logFile->open(QIODevice::Append | QIODevice::Text))
        {
           QMessageBox msgBox;
           msgBox.setText(tr("Nie mozna otworzyc pliku logowania!"));
           msgBox.exec();
           return false;
        }
        else
        {
            QTextStream out(_logFile);
            out << logValue ;
            _logFile->close();
        }
    }
    return true;
}

bool Logger::logSimple(QLinkedList<SiftDescriptor*>& list)
{
    if(doLog)
    {
        if(!_logFile->open(QIODevice::Append | QIODevice::Text))
        {
           QMessageBox msgBox;
           msgBox.setText(tr("Nie mozna otworzyc pliku logowania!"));
           msgBox.exec();
           return false;
        }
        else
        {
            QTextStream out(_logFile);
            QMutableLinkedListIterator<SiftDescriptor *> i(list);
            while (i.hasNext())
            {
                SiftDescriptor * example = i.next();
                out << example->_name << "," << QString::number(example->_class) << ",";
                for (int i = 0; i < SiftDescriptor::_numberOfDesc; i++)
                    out << QString::number(*(example->_descriptor + i))<< ",";
                out << "\n";
            }
            _logFile->close();
        }
    }
    return true;
}

bool Logger::logSimple(QLinkedList<SiftImage*>& list, short wordsNumber, int mode)
{
    bool isLogged = false;
    if(doLog)
    {
        QMutableLinkedListIterator<SiftImage *> i(list);
        while (i.hasNext())
        {
            SiftImage * example = i.next();
            this->logSimple(example, wordsNumber, mode);
        }
    }
    return isLogged;
}

bool Logger::logSimple(SiftImage* img, short wordsNumber, int mode)
{
    bool isLogged = false;
    if(doLog)
    {
        int blockNum = 0;
        QMutableLinkedListIterator<int *> ei(img->_siftBth);
        QMutableLinkedListIterator<int *> si(img->_siftHistogram);
        QMutableLinkedListIterator<double *> ni(img->_siftHistogramN);
        while (ei.hasNext() && si.hasNext())
            isLogged = logSimple(ei.next(), si.next(), ni.next(), wordsNumber, img->_name, blockNum++, img->_featuresList.count(), mode);
    }
    return isLogged;
}


bool Logger::logDistance(QLinkedList<SiftImage*> &list, int amount)
{
    bool isLogged = false;
    if(doLog)
    {
        if(!_logFile->open(QIODevice::Append | QIODevice::Text))
        {
           QMessageBox msgBox;
           msgBox.setText(tr("Nie mozna otworzyc pliku logowania!"));
           msgBox.exec();
           return false;
        }
        else
        {
            QTextStream out(_logFile);
            out << "Analizowany obraz, <--klasa, klasa-->, Obraz porownywany, Odleglosc, Poprawnie, Blednie\n";
            _logFile->close();
            QMutableLinkedListIterator<SiftImage *> i(list);
            while (i.hasNext())
            {
                SiftImage * example = i.next();
                isLogged = logDistance(example->_name, example->_distances, example->_class, amount);
            }
        }
    }
    return isLogged;

}

bool Logger::logDistance(const QString &imageName, QMap<uint, int> &distances, int imageClass, int amount)
{
    if(doLog)
    {
        if(!_logFile->open(QIODevice::Append | QIODevice::Text))
        {
           QMessageBox msgBox;
           msgBox.setText(tr("Nie mozna otworzyc pliku logowania!"));
           msgBox.exec();
           return false;
        }
        else
        {
            QSqlQuery query;
            QTextStream out(_logFile);
            QMapIterator<uint, int> it(distances);
            uint prevKey = std::numeric_limits<uint>::max();
            while (it.hasNext())
            {
                uint key = it.peekNext().key();
                int value = it.next().value();
                if (amount <= 0 && prevKey != key)
                    break;
                amount--;
                prevKey = key;
                QString queryContent = "SELECT cls, file FROM images WHERE id = " + QString::number(value);
                query.exec(queryContent);
                query.next();
                QSqlRecord record = query.record();
                QString fileQString = record.value("file").toString();
                int cls = record.value("cls").toInt();
                out << imageName << ", " << imageClass << ", "  << cls << ", "  << fileQString << ", " << key << ", " << QString::number((imageClass==cls)?1:0) << ", " << QString::number((imageClass!=cls)?1:0) << "\n";

            }
            _logFile->close();
            return true;
        }


    }
    return false;
}

bool Logger::logSimpleSIFT(QLinkedList<SiftImage*>& list)
{
    bool isLogged = false;
    if(doLog)
    {
        QMutableLinkedListIterator<SiftImage *> i(list);

        while (i.hasNext())
        {
            SiftImage * example = i.next();
            isLogged = logSimple(example->_featuresList);
        }
    }
    return isLogged;
}

bool Logger::logSimple(int *siftBth, int * siftHistogram, double * siftHistogramN, int n, const QString& name, int blockNum, int siftNum, int mode)
{
    if(doLog)
    {
        if(!_logFile->open(QIODevice::Append | QIODevice::Text))
        {
           QMessageBox msgBox;
           msgBox.setText(tr("Nie mozna otworzyc pliku logowania!"));
           msgBox.exec();
           return false;
        }
        else
        {
            QTextStream out(_logFile);
            if (mode == 0 || mode == 1)
            {
                out << name << ", " + QString::number(siftNum) << ", SIFT Block " + QString::number(blockNum) +": ,";
                for (int i = 0; i < n; i++)
                    out << QString::number(*(siftHistogram + i)) << ", ";
                out << "\n";
            }
            if (mode == 0 || mode == 2)
            {
                out << name << ", " + QString::number(siftNum) << ", BTH Block " + QString::number(blockNum) +": ,";
                for (int i = 0; i < n; i++)
                    out << QString::number(*(siftBth + i)) << ", ";
                out << "\n";
            }
            if (mode == 0 || mode == 3)
            {
                out << name << ", " + QString::number(siftNum) << ", BTH Block " + QString::number(blockNum) +": ,";
                for (int i = 0; i < n; i++)
                    out << QString::number(*(siftHistogramN + i)) << ", ";
                out << "\n";
            }
            _logFile->close();
        }
    }
    return true;
}

bool Logger::clearLogFile()
{
    if(doLog)
    {
        if(!_logFile->open(QIODevice::WriteOnly | QIODevice::Text))
        {
           QMessageBox msgBox;
           msgBox.setText(tr("Nie mozna otworzyc pliku logowania!"));
           msgBox.exec();
           return false;
        }
        else
        {
            _logFile->close();
        }
    }
    return true;
}

bool Logger::logBth(const QString & name, int count, int * bth)
{
    if(doLog)
    {

        if(!_logFile->open(QIODevice::Append | QIODevice::Text))
        {
           QMessageBox msgBox;
           msgBox.setText(tr("Nie mozna otworzyc pliku logowania!"));
           msgBox.exec();
           return false;
        }
        else
        {
            QTextStream out(_logFile);
            out << name << " - BTH: ";
            for(int i = 0; i < count; i++)
                out << QString::number(*(bth+i)) << ", ";
            out << "\n";
            _logFile->close();
        }
    }
    return true;
}
