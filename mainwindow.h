#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets>
#include <QColorDialog>
#include "view.h"
#include "analyser.h"
#include "imageanalyser.h"
#include "bthanalyser.h"
#include "bthareaanalyser.h"
#include "siftanalyser.h"
#include "lssanalyser.h"
#include "logger.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:

public slots:
    void siftBorderExtractionButtonClicked();
    void siftFindClosestImagesButtonClicked();
    void enableSIFTDictButtons();
    void enableSIFTDivButtons();

private:
    void createStatusBar();
    void createTips();
    void createConnections();
    void createAnalysers();
    void sendUserParameters(const bool &);
    Ui::MainWindow *ui;
    View *myGraphicView;

    QTabWidget *tabWidget;

    // SIFT
    QWidget *SIFTPage;
    QGridLayout *SIFTLayout;
    QPushButton *LSSSearchButton;
    QPushButton *SIFTDrawImagesButton;
    QPushButton *SIFTSearchButton;
    SiftAnalyser *siftAnalyser;
    QCheckBox *SIFTNewDictionaryCheckBox;
    QLineEdit *SIFTThresholdValueEdit;
    QLabel *SIFTThresholdLabel;
    QLineEdit *SIFTBlockNumWEdit;
    QLabel *SIFTBlockNumWLabel;
    QLineEdit *SIFTBlockNumHEdit;
    QLabel *SIFTBlockNumHLabel;
    QLineEdit *SIFTQueryXBlockEdit;
    QLabel *SIFTQueryXBlockLabel;
    QLineEdit *SIFTQueryYBlockEdit;
    QLabel *SIFTQueryYBlockLabel;
    QLineEdit *SIFTQueryMaxXBlockEdit;
    QLabel *SIFTQueryMaxXBlockLabel;
    QLineEdit *SIFTQueryMaxYBlockEdit;
    QLabel *SIFTQueryMaxYBlockLabel;
    QLineEdit *SIFTImageNumEdit;
    QLabel *SIFTImageNumLabel;
    QLineEdit *SIFTMinValueEdit;
    QLineEdit *SIFTMaxValueEdit;
    QHBoxLayout * SIFTBlocksLayout;
    QVBoxLayout * SIFTBlocksWLayout;
    QVBoxLayout * SIFTBlocksHLayout;
    QHBoxLayout * SIFTMinMaxLayout;
    QVBoxLayout * SIFTImageNumLayout;
    QHBoxLayout * SIFTDrawImagesLayout;
    QVBoxLayout * SIFTThresholdAndNewDictLayout;
    QVBoxLayout * SIFTNewDictLayout;
    QVBoxLayout * SIFTDictMethodsLayout;
    QHBoxLayout * SIFTDistMethodLayout;
    QHBoxLayout * SIFTDivMethodLayout;

    QLabel * SIFTDistanceLabel;
    QLineEdit * SIFTDistanceEdit;
    QLabel * SIFTWordsNumberLabel;
    QLineEdit * SIFTWordsNumberEdit;
    QRadioButton * SIFTDictMethodMine;
    QRadioButton * SIFTDictMethodHartigan;
    QRadioButton * SIFTDistBTH;
    QRadioButton * SIFTDistMin;
    QRadioButton * SIFTDivBlock;
    QRadioButton * SIFTDivPyr;
    // LSS
    LssAnalyser * lssAnalyser;

};

#endif // MAINWINDOW_H
