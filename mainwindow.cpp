#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    myGraphicView(new View("images")),
    tabWidget(new QTabWidget(this)),
    // SIFT
    SIFTPage(new QWidget()),
    SIFTLayout(new QGridLayout(this)),
    LSSSearchButton(new QPushButton("Znajdź podobne obrazy(LSS)", this)),
    SIFTSearchButton(new QPushButton("Znajdź podobne obrazy(SIFT)", this)),
    SIFTNewDictionaryCheckBox(new QCheckBox("Nowy słownik", this)),
    SIFTThresholdValueEdit(new QLineEdit("0.5", this)),
    SIFTThresholdLabel(new QLabel("Wartość progu", this)),
    SIFTBlockNumWEdit(new QLineEdit("1", this)),
    SIFTBlockNumWLabel(new QLabel("Bloki szer.", this)),
    SIFTBlockNumHEdit(new QLineEdit("1", this)),
    SIFTBlockNumHLabel(new QLabel("Bloki wys.", this)),
    SIFTQueryXBlockEdit(new QLineEdit("0", this)),
    SIFTQueryXBlockLabel(new QLabel("Pozycja X", this)),
    SIFTQueryYBlockEdit(new QLineEdit("0", this)),
    SIFTQueryYBlockLabel(new QLabel("Pozycja Y", this)),
    SIFTQueryMaxXBlockEdit(new QLineEdit("0", this)),
    SIFTQueryMaxXBlockLabel(new QLabel("Max Pozycja X", this)),
    SIFTQueryMaxYBlockEdit(new QLineEdit("0", this)),
    SIFTQueryMaxYBlockLabel(new QLabel("Max Pozycja Y", this)),
    SIFTImageNumEdit(new QLineEdit("1", this)),
    SIFTImageNumLabel(new QLabel("Ilosc obrazow los.", this)),
    SIFTMinValueEdit(new QLineEdit("0", this)),
    SIFTMaxValueEdit(new QLineEdit("255", this)),
    SIFTBlocksLayout(new QHBoxLayout(this)),
    SIFTBlocksWLayout(new QVBoxLayout(this)),
    SIFTBlocksHLayout(new QVBoxLayout(this)),
    SIFTMinMaxLayout(new QHBoxLayout(this)),
    SIFTImageNumLayout(new QVBoxLayout(this)),
    SIFTDrawImagesLayout(new QHBoxLayout(this)),
    SIFTThresholdAndNewDictLayout(new QVBoxLayout(this)),
    SIFTNewDictLayout(new QVBoxLayout(this)),
    SIFTDictMethodsLayout(new QVBoxLayout(this)),
    SIFTDistMethodLayout(new QHBoxLayout(this)),
    SIFTDivMethodLayout(new QHBoxLayout(this)),
    SIFTDistanceLabel(new QLabel(tr("Liczba obrazów odpowiedzi"), this)),
    SIFTDistanceEdit(new QLineEdit("30", this)),
    SIFTWordsNumberLabel(new QLabel("Liczba słów w słowniku", this)),
    SIFTWordsNumberEdit(new QLineEdit("200", this)),
    SIFTDictMethodMine(new QRadioButton("Autorska kMeans",this)),
    SIFTDictMethodHartigan(new QRadioButton("Hartigan kMeans", this)),
    SIFTDistBTH(new QRadioButton("BTH", this)),
    SIFTDistMin(new QRadioButton("Min", this)),
    SIFTDivBlock(new QRadioButton("Bloki", this)),
    SIFTDivPyr(new QRadioButton("Piramida", this))
{
    ui->setupUi(this);
    createAnalysers();
    createConnections();
    createTips();
    createStatusBar();

    ui->menuLayout->setSpacing(5);
    ui->menuLayout->addWidget(tabWidget, 0, 0);

    // SIFT
    SIFTBlocksWLayout->addWidget(SIFTBlockNumWLabel);
    SIFTBlocksWLayout->addWidget(SIFTBlockNumWEdit);
    SIFTBlocksWLayout->addWidget(SIFTQueryXBlockLabel);
    SIFTBlocksWLayout->addWidget(SIFTQueryXBlockEdit);
    SIFTBlocksWLayout->addWidget(SIFTQueryMaxXBlockLabel);
    SIFTBlocksWLayout->addWidget(SIFTQueryMaxXBlockEdit);

    SIFTBlocksHLayout->addWidget(SIFTBlockNumHLabel);
    SIFTBlocksHLayout->addWidget(SIFTBlockNumHEdit);
    SIFTBlocksHLayout->addWidget(SIFTQueryYBlockLabel);
    SIFTBlocksHLayout->addWidget(SIFTQueryYBlockEdit);
    SIFTBlocksHLayout->addWidget(SIFTQueryMaxYBlockLabel);
    SIFTBlocksHLayout->addWidget(SIFTQueryMaxYBlockEdit);

    SIFTBlocksLayout->addLayout(SIFTBlocksWLayout);
    SIFTBlocksLayout->addLayout(SIFTBlocksHLayout);

    SIFTMinMaxLayout->addWidget(SIFTMinValueEdit);
    SIFTMinMaxLayout->addWidget(SIFTMaxValueEdit);
    SIFTMinValueEdit->setVisible(false);
    SIFTMaxValueEdit->setVisible(false);

    QGroupBox * SIFTDictMethodGroupBox = new QGroupBox("Metoda tworzenia słownika");
    SIFTDictMethodsLayout->addWidget(SIFTNewDictionaryCheckBox);
    SIFTDictMethodsLayout->addWidget(SIFTDictMethodMine);
    SIFTDictMethodsLayout->addWidget(SIFTDictMethodHartigan);
    SIFTDictMethodGroupBox->setLayout(SIFTDictMethodsLayout);

    SIFTThresholdAndNewDictLayout->addWidget(SIFTDictMethodGroupBox);
    SIFTThresholdAndNewDictLayout->addWidget(SIFTWordsNumberLabel);
    SIFTThresholdAndNewDictLayout->addWidget(SIFTWordsNumberEdit);
    SIFTThresholdAndNewDictLayout->addWidget(SIFTThresholdLabel);
    SIFTThresholdAndNewDictLayout->addWidget(SIFTThresholdValueEdit);

    QGroupBox * SIFTDistMethodGroupBox = new QGroupBox("Metoda liczenia odległości");
    SIFTDistMethodLayout->addWidget(SIFTDistBTH);
    SIFTDistMethodLayout->addWidget(SIFTDistMin);
    SIFTDistMethodGroupBox->setLayout(SIFTDistMethodLayout);

    QGroupBox * SIFTDivMethodGroupBox = new QGroupBox("Metoda podziału obrazka");
    SIFTDivMethodLayout->addWidget(SIFTDivBlock);
    SIFTDivMethodLayout->addWidget(SIFTDivPyr);
    SIFTDivMethodGroupBox->setLayout(SIFTDivMethodLayout);

    SIFTImageNumLabel->setVisible(false);
    SIFTImageNumEdit->setVisible(false);

    SIFTDrawImagesLayout->addLayout(SIFTImageNumLayout);

    int row = 0;
    int col = 0;
    SIFTPage->setLayout(SIFTLayout);
    SIFTLayout->addWidget(LSSSearchButton, row++, col);
    SIFTLayout->addWidget(SIFTSearchButton, row++, col);

    SIFTLayout->addLayout(SIFTThresholdAndNewDictLayout, row++, col);
    SIFTLayout->addWidget(SIFTDistMethodGroupBox, row++, col);
    SIFTLayout->addWidget(SIFTDivMethodGroupBox, row++, col);
    SIFTLayout->addLayout(SIFTBlocksLayout, row++, col);
    SIFTLayout->addWidget(SIFTDistanceLabel, row++, col);
    SIFTLayout->addWidget(SIFTDistanceEdit, row++, col);

    SIFTDistBTH->setChecked(true);
    SIFTDictMethodMine->setChecked(true);
    SIFTNewDictionaryCheckBox->setChecked(true);
    SIFTDivBlock->setChecked(true);
    // dodawanie zakladek
    tabWidget->setMaximumHeight(700);
    tabWidget->setMaximumWidth(250) ;
    tabWidget->addTab(SIFTPage, "SIFT\\LSS");

    setWindowTitle("System nawigacji graficznej");
}

MainWindow::~MainWindow()
{
    QSqlDatabase db;
    db.close();
    delete ui;
}


void MainWindow::siftBorderExtractionButtonClicked()
{
    QString lssFileName = QFileDialog::getOpenFileName(this,
             tr("Open Image"), "./images/", tr("Image Files (*.png *.jpg *.bmp *.tif)"));
    if (lssFileName != "")
    {
        this->sendUserParameters(false);
        this->lssAnalyser->analyseImageAndFindClosestImages(lssFileName);
    }


}

void MainWindow::siftFindClosestImagesButtonClicked()
{
    _BTHFileName = QFileDialog::getOpenFileName(this,
             tr("Wybierz Obraz"), "./images/", tr("Pliki Obrazów (*.png *.jpg *.bmp *.tif)"));
    if (_BTHFileName != "")
    {
        if (this->siftAnalyser->_siftDictionary.isEmpty() || this->siftAnalyser->_siftImages.isEmpty())
        {
            this->siftAnalyser->readDictConfFile("./SIFT_Images_96_"+ QString::number(this->SIFTWordsNumberEdit->text().toInt()) + "_dict.txt");
            this->siftAnalyser->readImgConfFile("./SIFT_Images_96.txt");
        }
        if (this->siftAnalyser->_siftDictionary.size() != this->SIFTWordsNumberEdit->text().toInt())
            this->siftAnalyser->setNewDictionary(true);
        else if (this->siftAnalyser->_thresholdValue != this->SIFTThresholdValueEdit->text().toInt()
                 || this->siftAnalyser->_blockNumH != this->SIFTBlockNumHEdit->text().toInt()
                 || this->siftAnalyser->_blockNumW != this->SIFTBlockNumWEdit->text().toInt()
                 )
            this->siftAnalyser->setOnlyBTH(true);
        sendUserParameters(true);
        this->siftAnalyser->analyseAndFindClosestImages(_BTHFileName);

        InfoWindow *window;
        QImage *image = new QImage(_BTHFileName);
        window = new InfoWindow(*image, this);
        window->setWindowTitle("Obraz query");
        window->show();
    }
}

void MainWindow::createStatusBar()
{
    ui->statusBar->setEnabled(true);
    ui->statusBar->showMessage("Witaj!", 2000);
}

void MainWindow::createConnections()
{
    connect(LSSSearchButton, SIGNAL(clicked()), this, SLOT(siftBorderExtractionButtonClicked()));
    connect(SIFTSearchButton, SIGNAL(clicked()), this, SLOT(siftFindClosestImagesButtonClicked()));
    connect(lssAnalyser, SIGNAL(sendIdValue(QString)), myGraphicView, SLOT(updateItemsSift(QString)));
    connect(SIFTNewDictionaryCheckBox, SIGNAL(clicked()), this, SLOT(enableSIFTDictButtons()));
    connect(SIFTDivBlock, SIGNAL(clicked()),this, SLOT(enableSIFTDivButtons()));
    connect(SIFTDivPyr, SIGNAL(clicked()),this, SLOT(enableSIFTDivButtons()));
}

void MainWindow::createAnalysers()
{
    colorWindow = new QColorDialog(this);
    siftAnalyser = new SiftAnalyser("SIFT_Images.txt", "images");
    lssAnalyser = new LssAnalyser("LSS_Images.txt", "images");
    this->sendUserParameters(false);

}

void MainWindow::createTips()
{
}

void MainWindow::sendUserParameters(const bool &blocks)
{
    bool ok = true;
    double threshold = 4;
    int imgNum = 20;
    int minVal = 0;
    int maxVal = 255;
    int blockNumH = 1;
    int blockNumW = 1;
    short wordsNum = 200;
    int distance = 40;
    int queryBlockX= 0;
    int queryBlockY = 0;
    int queryBlockXMax = 0;
    int queryBlockYMax = 0;

    // sprawdzam czy wartosc progu w textedit nie jest pusta
    if (this->SIFTThresholdValueEdit->text().isEmpty())
        this->SIFTThresholdValueEdit->setText(QString::number(4));
    if (this->SIFTImageNumEdit->text().isEmpty())
        this->SIFTImageNumEdit->setText(QString::number(1000));
    if (this->SIFTMinValueEdit->text().isEmpty())
        this->SIFTMinValueEdit->setText(QString::number(0));
    if (this->SIFTMaxValueEdit->text().isEmpty())
        this->SIFTMaxValueEdit->setText(QString::number(255));
    if (this->SIFTDistanceEdit->text().isEmpty())
        this->SIFTDistanceEdit->setText(QString::number(40));
    if (this->SIFTWordsNumberEdit->text().isEmpty())
        this->SIFTWordsNumberEdit->setText(QString::number(200));
    // proba pobrania wartosci z pola
    threshold = this->SIFTThresholdValueEdit->text().toDouble(&ok);
    if (ok) 
    {
        siftAnalyser->setThresholdValue(threshold);
        lssAnalyser->setThresholdValue(threshold);
    }
    imgNum = this->SIFTImageNumEdit->text().toInt(&ok);
    if (ok)
        siftAnalyser->setImagesNum(abs(imgNum));
    minVal = this->SIFTMinValueEdit->text().toInt(&ok);
    if (ok)
        siftAnalyser->setMinValue(minVal);
    maxVal = this->SIFTMaxValueEdit->text().toInt(&ok);
    if (ok)
        siftAnalyser->setMaxValue(maxVal);
    wordsNum = this->SIFTWordsNumberEdit->text().toShort(&ok);
    if (ok)
    {
        siftAnalyser->setNumberOfWords(wordsNum);
        lssAnalyser->setDictWordsNum(wordsNum);
    }
    distance = this->SIFTDistanceEdit->text().toInt(&ok);
    if (ok)
    {
        siftAnalyser->setDistance(distance);
        lssAnalyser->setDistance((uint)distance);
    }
    if (blocks)
    {
        if (this->SIFTBlockNumHEdit->text().isEmpty())
            this->SIFTBlockNumHEdit->setText(QString::number(1));
        if (this->SIFTBlockNumWEdit->text().isEmpty())
            this->SIFTBlockNumWEdit->setText(QString::number(1));
        if (this->SIFTQueryXBlockEdit->text().isEmpty())
            this->SIFTQueryXBlockEdit->setText(QString::number(1));
        if (this->SIFTQueryYBlockEdit->text().isEmpty())
            this->SIFTQueryYBlockEdit->setText(QString::number(1));
        if (this->SIFTQueryMaxXBlockEdit->text().isEmpty())
            this->SIFTQueryMaxXBlockEdit->setText(QString::number(1));
        if (this->SIFTQueryMaxYBlockEdit->text().isEmpty())
            this->SIFTQueryMaxYBlockEdit->setText(QString::number(1));


        blockNumH = this->SIFTBlockNumHEdit->text().toInt(&ok);
        if (ok)
            siftAnalyser->setBlockNumH(blockNumH);

        blockNumW = this->SIFTBlockNumWEdit->text().toInt(&ok);
        if (ok)
            siftAnalyser->setBlockNumW(blockNumW);

        queryBlockX = this->SIFTQueryXBlockEdit->text().toInt(&ok);
        if (ok)
            siftAnalyser->setQueryXBlock(queryBlockX);

        queryBlockY = this->SIFTQueryYBlockEdit->text().toInt(&ok);
        if (ok)
            siftAnalyser->setQueryYBlock(queryBlockY);

        queryBlockXMax = this->SIFTQueryMaxXBlockEdit->text().toInt(&ok);
        if (ok)
            siftAnalyser->setQueryXMaxBlock(queryBlockXMax);

        queryBlockYMax = this->SIFTQueryMaxYBlockEdit->text().toInt(&ok);
        if (ok)
            siftAnalyser->setQueryYMaxBlock(queryBlockYMax);

    }

    siftAnalyser->setNewDictionary(this->SIFTNewDictionaryCheckBox->isChecked());
    if (this->SIFTDictMethodMine->isChecked())
        this->siftAnalyser->setDictionaryInitMethod(0);
    else if(this->SIFTDictMethodHartigan->isChecked())
        this->siftAnalyser->setDictionaryInitMethod(1);

    if (this->SIFTDistBTH->isChecked())
        this->siftAnalyser->setDistanceMethod(0);
    else if(this->SIFTDistMin->isChecked())
        this->siftAnalyser->setDistanceMethod(1);

    if (this->SIFTDivBlock->isChecked())
        this->siftAnalyser->setDivisionMethod(0);
    else if(this->SIFTDivPyr->isChecked())
        this->siftAnalyser->setDivisionMethod(1);
}

void MainWindow::enableSIFTDictButtons()
{
    this->SIFTDictMethodHartigan->setEnabled(this->SIFTNewDictionaryCheckBox->isChecked());
    this->SIFTDictMethodMine->setEnabled(this->SIFTNewDictionaryCheckBox->isChecked());
    this->SIFTWordsNumberEdit->setEnabled(this->SIFTNewDictionaryCheckBox->isChecked());
    this->SIFTWordsNumberLabel->setEnabled(this->SIFTNewDictionaryCheckBox->isChecked());
}

void MainWindow::enableSIFTDivButtons()
{
    this->SIFTBlockNumHEdit->setEnabled(this->SIFTDivBlock->isChecked());
    this->SIFTBlockNumHLabel->setEnabled(this->SIFTDivBlock->isChecked());
    this->SIFTBlockNumWEdit->setEnabled(this->SIFTDivBlock->isChecked());
    this->SIFTBlockNumWLabel->setEnabled(this->SIFTDivBlock->isChecked());
    this->SIFTQueryXBlockEdit->setEnabled(this->SIFTDivBlock->isChecked());
    this->SIFTQueryXBlockLabel->setEnabled(this->SIFTDivBlock->isChecked());
    this->SIFTQueryYBlockEdit->setEnabled(this->SIFTDivBlock->isChecked());
    this->SIFTQueryYBlockLabel->setEnabled(this->SIFTDivBlock->isChecked());
    this->SIFTQueryMaxXBlockEdit->setEnabled(this->SIFTDivBlock->isChecked());
    this->SIFTQueryMaxXBlockLabel->setEnabled(this->SIFTDivBlock->isChecked());
    this->SIFTQueryMaxYBlockEdit->setEnabled(this->SIFTDivBlock->isChecked());
    this->SIFTQueryMaxYBlockLabel->setEnabled(this->SIFTDivBlock->isChecked());
}

