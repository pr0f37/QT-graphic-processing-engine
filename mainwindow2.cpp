#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    myGraphicView(new View("images")),
    BNPage(new QWidget()),
    BTHPage(new QWidget()),
    BTHInitializeButton(new QPushButton("Przeanalizuj obrazy", this)),
    BTHLoadButton(new QPushButton("Wczytaj analize", this)),
    BTHSearchButton(new QPushButton("Szukaj", this)),
    BTHLayout(new QGridLayout(this)),
    BNMenuLayout(new QGridLayout(this)),
    tabWidget(new QTabWidget(this)),
    codeLabel(new QLabel("Indeks:", this, 0)),
    codeEdit(new QLineEdit("0", this)),
    codeButton(new QPushButton("Szukaj wg indeksu", this)),
    colorButton(new QPushButton("Szukaj wg koloru", this)),
    exampleButton(new QPushButton("Szukaj wg przykladu", this)),
    componentLabel(new QLabel("Komponent koloru:", this, 0)),
    rgComponentButton(new QRadioButton("RG", this)),
    byComponentButton(new QRadioButton("BY", this)),
    reBstTreeButton(new QPushButton("Odtworz BST", this)),
    radioLayout(new QHBoxLayout(this)),
    treeButtonsLayout(new QHBoxLayout(this)),
    upButton(new QPushButton(QPixmap(":/up"), "", this)),
    rightButton(new QPushButton(QPixmap(":/right"), "", this)),
    leftButton(new QPushButton(QPixmap(":/left"), "", this)),
    treeDepthLabel(new QLabel("Glebokosc BST:")),
    treeDepthEdit(new QLineEdit("4", this)),
    imageQuarterLabel(new QLabel("Cwiartka:")),
    imageQuarterEdit(new QLineEdit("0", this)),
    shiftLabel(new QLabel("Przesuniecie BST:")),
    shiftEdit(new QLineEdit("0", this)),
    shiftSlider(new QSlider(Qt::Horizontal, this)),
    BTHSlider(new QSlider(Qt::Horizontal, this)),
    BTHSliderLabel(new QLabel("Prog:", this)),
    BTHSliderValue(new QLineEdit("0", this)),
    BTHAreaButton(new QPushButton("Ustaw OZ", this)),
    BTHRepeatButton(new QPushButton("Powtorz szukanie", this)),
    BTHAreaIndicator(new QCheckBox("Obszar zainter.", this)),
    BTHAreaWidthEdit(new QLineEdit("1", this)),
    BTHAreaHeightEdit(new QLineEdit("1", this)),
    BTHAreaPosXEdit(new QLineEdit("0", this)),
    BTHAreaPosYEdit(new QLineEdit("0", this)),
    BTHAreaWidthLabel(new QLabel("Szerokosc:")),
    BTHAreaHeightLabel(new QLabel("Wysokosc:")),
    BTHAreaPosXLabel(new QLabel("Pozycja X:")),
    BTHAreaPosYLabel(new QLabel("Pozycja Y:")),
    BTHAreaLayout(new QGridLayout(this)),
    // SIFT
    SIFTPage(new QWidget()),
    SIFTLayout(new QGridLayout(this)),
    SIFTDictRandButton(new QPushButton("Utw. slownika(losowanie)", this)),
    SIFTKmeansButton(new QPushButton("Utw. slownika(Kmeans)", this)),
    SIFTDrawImagesButton(new QPushButton("Losowanie obrazow", this)),
    SIFTFindClosestImagesButton(new QPushButton("Znajdz podobne obrazy", this)),
    SIFTNewDictionaryCheckBox(new QCheckBox("Nowy slownik", this)),
    SIFTThresholdValueEdit(new QLineEdit("4", this)),
    SIFTThresholdLabel(new QLabel("Wart. progu", this)),
    SIFTBlockNumWEdit(new QLineEdit("1", this)),
    SIFTBlockNumWLabel(new QLabel("Bloki szer.", this)),
    SIFTBlockNumHEdit(new QLineEdit("1", this)),
    SIFTBlockNumHLabel(new QLabel("Bloki wys.", this)),
    SIFTImageNumEdit(new QLineEdit("1", this)),
    SIFTImageNumLabel(new QLabel("Ilosc obrazow los.", this)),
    SIFTMinValueEdit(new QLineEdit("0", this)),
    SIFTMaxValueEdit(new QLineEdit("255", this)),
    SIFTVectorNumLabel(new QLabel("Ilosc wektorow SIFT", this)),
    SIFTVectorNumEdit(new QLineEdit("1000", this)),
    SIFTBlocksLayout(new QHBoxLayout(this)),
    SIFTBlocksWLayout(new QVBoxLayout(this)),
    SIFTBlocksHLayout(new QVBoxLayout(this)),
    SIFTMinMaxLayout(new QHBoxLayout(this)),
    SIFTThresholdLayout(new QVBoxLayout(this)),
    SIFTImageNumLayout(new QVBoxLayout(this)),
    SIFTDrawImagesLayout(new QHBoxLayout(this)),
    SIFTThresholdAndNewDictLayout(new QHBoxLayout(this)),
    SIFTDistanceLabel(new QLabel("Odleglosc.", this)),
    SIFTDistanceEdit(new QLineEdit("30", this)),
    SIFTWordsNumberLabel(new QLabel("Liczba slow w slowniku", this)),
    SIFTWordsNumberEdit(new QLineEdit("200", this))
{
    ui->setupUi(this);
    byComponentButton->setChecked(true);

    createAnalysers();
    createConnections();
    createTips();
    createStatusBar();

    ui->menuLayout->setSpacing(5);
    ui->menuLayout->addWidget(tabWidget, 0, 0);

    BNPage->setLayout(BNMenuLayout);
    shiftSlider->setMinimum(-511);
    shiftSlider->setMaximum(511);
    shiftSlider->setValue(0);

    BNMenuLayout->setSpacing(5);
    BNMenuLayout->addWidget(componentLabel, 0, 0);
    BNMenuLayout->addLayout(radioLayout, 1, 0);
    BNMenuLayout->addWidget(treeDepthLabel, 2, 0);
    BNMenuLayout->addWidget(treeDepthEdit, 3, 0);
    BNMenuLayout->addWidget(reBstTreeButton, 4, 0);
    BNMenuLayout->addWidget(imageQuarterLabel, 6, 0);
    BNMenuLayout->addWidget(imageQuarterEdit, 7, 0);
    BNMenuLayout->addWidget(shiftLabel, 8, 0);
    BNMenuLayout->addWidget(shiftSlider, 9, 0);
    BNMenuLayout->addWidget(shiftEdit, 10, 0);
    BNMenuLayout->addWidget(codeLabel, 11, 0);
    BNMenuLayout->addWidget(codeEdit, 12, 0);
    BNMenuLayout->addWidget(codeButton, 13, 0);
    BNMenuLayout->addWidget(colorButton, 14, 0);
    BNMenuLayout->addWidget(exampleButton, 15, 0);
    BNMenuLayout->addLayout(treeButtonsLayout, 16, 0);

    enableBSTButtons(false);

    treeButtonsLayout->addWidget(leftButton);
    treeButtonsLayout->addWidget(upButton);
    treeButtonsLayout->addWidget(rightButton);

    radioLayout->addWidget(rgComponentButton);
    radioLayout->addWidget(byComponentButton);

    ui->gridLayout->addWidget(myGraphicView);

    myGraphicView->showFullScreen();
    myGraphicView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    myGraphicView->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    BTHSlider->setMinimum(0);
    BTHSlider->setMaximum(500);
    BTHSlider->setValue(500);

    enableBTHButtons(false);

    BTHPage->setLayout(BTHLayout);
    BTHLayout->addWidget(BTHInitializeButton, 0, 0);
    BTHLayout->addWidget(BTHLoadButton, 1, 0);
    BTHLayout->addWidget(BTHAreaIndicator, 2, 0);
    BTHLayout->addWidget(BTHAreaButton, 3, 0);


    BTHLayout->addLayout(BTHAreaLayout, 4, 0);

    BTHLayout->addWidget(BTHSearchButton, 5, 0);
    BTHLayout->addWidget(BTHRepeatButton, 6, 0);
    BTHLayout->addWidget(BTHSliderLabel, 7, 0);
    BTHLayout->addWidget(BTHSlider, 8, 0);
    BTHLayout->addWidget(BTHSliderValue, 9, 0);

    BTHAreaWidthEdit->setMaximumWidth(20);
    BTHAreaHeightEdit->setMaximumWidth(20);
    BTHAreaPosXEdit->setMaximumWidth(20);
    BTHAreaPosYEdit->setMaximumWidth(20);
    BTHAreaLayout->addWidget(BTHAreaWidthLabel, 0, 0);
    BTHAreaLayout->addWidget(BTHAreaWidthEdit, 0, 1);
    BTHAreaLayout->addWidget(BTHAreaHeightLabel, 1, 0);
    BTHAreaLayout->addWidget(BTHAreaHeightEdit, 1, 1);
    BTHAreaLayout->addWidget(BTHAreaPosXLabel, 2, 0);
    BTHAreaLayout->addWidget(BTHAreaPosXEdit, 2, 1);
    BTHAreaLayout->addWidget(BTHAreaPosYLabel, 3, 0);
    BTHAreaLayout->addWidget(BTHAreaPosYEdit, 3, 1);
    // SIFT
    SIFTBlocksWLayout->addWidget(SIFTBlockNumWLabel);
    SIFTBlocksWLayout->addWidget(SIFTBlockNumWEdit);
    SIFTBlocksHLayout->addWidget(SIFTBlockNumHLabel);
    SIFTBlocksHLayout->addWidget(SIFTBlockNumHEdit);

    SIFTBlocksLayout->addLayout(SIFTBlocksWLayout);
    SIFTBlocksLayout->addLayout(SIFTBlocksHLayout);

    SIFTMinMaxLayout->addWidget(SIFTMinValueEdit);
    SIFTMinMaxLayout->addWidget(SIFTMaxValueEdit);

    SIFTThresholdLayout->addWidget(SIFTThresholdLabel);
    SIFTThresholdLayout->addWidget(SIFTThresholdValueEdit);

    SIFTThresholdAndNewDictLayout->addLayout(SIFTThresholdLayout);
    SIFTThresholdAndNewDictLayout->addWidget(SIFTNewDictionaryCheckBox);

    //SIFTImageNumLayout->addWidget(SIFTImageNumLabel);
    //SIFTImageNumLayout->addWidget(SIFTImageNumEdit);

    //SIFTDrawImagesLayout->addWidget(SIFTDrawImagesButton);
    SIFTDrawImagesLayout->addLayout(SIFTImageNumLayout);


    SIFTPage->setLayout(SIFTLayout);
    SIFTLayout->addWidget(SIFTDictRandButton, 0, 0);//, 10, 40);
    SIFTLayout->addWidget(SIFTKmeansButton, 1, 0);//, 10, 40);
    SIFTLayout->addWidget(SIFTFindClosestImagesButton, 2, 0);

    SIFTLayout->addLayout(SIFTThresholdAndNewDictLayout, 3, 0);
    SIFTLayout->addWidget(SIFTDistanceLabel, 4, 0);
    SIFTLayout->addWidget(SIFTDistanceEdit, 5, 0);

    SIFTLayout->addLayout(SIFTBlocksLayout, 6, 0);

    SIFTLayout->addLayout(SIFTDrawImagesLayout, 7, 0);

    //SIFTLayout->addWidget(SIFTVectorNumLabel, 8, 0);
    //SIFTLayout->addWidget(SIFTVectorNumEdit, 9, 0);
    //SIFTLayout->addLayout(SIFTMinMaxLayout, 10, 0);
    SIFTLayout->addWidget(SIFTWordsNumberLabel, 11, 0);
    SIFTLayout->addWidget(SIFTWordsNumberEdit, 12, 0);

    SIFTNewDictionaryCheckBox->setChecked(false);
    // dodawanie zakladek
    tabWidget->setMaximumHeight(400);
    tabWidget->setMaximumWidth(150) ;
    tabWidget->addTab(SIFTPage, "SIFT");
    tabWidget->addTab(BNPage,"BST");
    tabWidget->addTab(BTHPage, "BTH");

    tabWidget->setTabEnabled(1,false);
    tabWidget->setTabEnabled(2,false);
    setWindowTitle("System nawigacji graficznej");
}

void MainWindow::getFileName()
{
    if (checkQuarter((imageQuarterEdit->text()).toInt()) && checkShift(shiftEdit->text().toInt()))
    {
        emit sendShift(shiftEdit->text().toInt());
        emit sendQuarter((imageQuarterEdit->text()).toInt());
        QString fileName = QFileDialog::getOpenFileName(this,
                         tr("Open Image"), "/home/", tr("Image Files (*.png *.jpg *.bmp *.tif)"));
        emit sendFileName(fileName);
    }
}

void MainWindow::getFileNameBTH(const QString action)
{
    Logger *logg = new Logger("getFileNameBTH.txt");
    if (BTHAreaIndicator->isChecked() != true)
    {
        if (action == "get")
            _BTHFileName = QFileDialog::getOpenFileName(this,
                     tr("Open Image"), "/home/", tr("Image Files (*.png *.jpg *.bmp *.tif)"));
        logg->log(_BTHFileName);
        emit sendThreesholdValue(BTHSliderValue->text().toInt());
        emit sendFileNameBTH(_BTHFileName);
        BTHRepeatButton->setEnabled(true);
    }
    else
    {
        int posX = BTHAreaPosXEdit->text().toInt();
        int posY = BTHAreaPosYEdit->text().toInt();
        int height = BTHAreaHeightEdit->text().toInt();
        int width = BTHAreaWidthEdit->text().toInt();
        int threeshold = BTHSliderValue->text().toInt();
        QMessageBox msgBox;

        if (posX < 0 || posX > 7)
        {
            msgBox.setText("Pozycja X obszaru zainteresowania powinna znajdowac sie w przedziale [0-7]");
            msgBox.exec();
        }
        else if (posY < 0 || posY > 4 )
        {
            msgBox.setText("Pozycja Y obszaru zainteresowania powinna znajdowa