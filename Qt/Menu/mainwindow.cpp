#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QLayout>
#include <iostream>

struct debugTagData
{
    QString name;
    bool terminal;
    bool logfile;
};

static debugTagData darray[]
{
    {"Tag1", false, false},
    {"Tag2", false, false},
    {"Tag3", false, false},
    {"Tag4", false, false},
    {"Tag5", false, false},
    {"Tag6", false, false},
    {"Tag7", false, false},
    {"Tag8", false, false},
    {"Tag9", false, false},
    {"Tag10", false, false},
};

constexpr uint32_t tagSize = sizeof(darray) / sizeof(darray[0]);
constexpr uint32_t dbgDalogWidth = 350;
constexpr uint32_t dbgDalogHigh = 300;
constexpr uint32_t dbgTableWidth = dbgDalogWidth;
constexpr uint32_t dbgTableHigh = dbgDalogHigh -30;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , debugDialog_(nullptr)
    , debugTags_(nullptr)
{
    ui->setupUi(this);

    connect(ui->actionDebug_Selection, &QAction::triggered, this, &MainWindow::showDebugDialog);

}

MainWindow::~MainWindow()
{
    delete debugTags_;
    delete debugDialog_;
    delete ui;
}

void MainWindow::showDebugDialog()
{
    if (debugDialog_==nullptr)
    {
        debugDialog_ = new QDialog(this);
        debugDialog_->setWindowTitle("Choose Debug Flags");
        debugDialog_->resize(dbgDalogWidth, dbgDalogHigh);
        if (debugTags_ == nullptr)
        {
            PopulateDebugTags(debugDialog_);
        }
        //debugDialog_->layout()->addWidget(debugTags_);
    }
    debugDialog_->exec();
}

void MainWindow::debugSelected(int row, int col)
{
    if(col == 1 || col == 2)
    {
        bool isChecked = debugTags_->item(row, col)->checkState() == Qt::Checked;
        // Update your internal array based on the new value of the checkbox
        if (col == 1)
        {
            darray[row].terminal = isChecked;
            std::cout << darray[row].name.toStdString() << " set terminal output as " << isChecked << "\n";
        }
        else
        {
            darray[row].logfile = isChecked;
            std::cout << darray[row].name.toStdString() << " set logfile output as " << isChecked << "\n";
        }
    }
}
void MainWindow::PopulateDebugTags(QWidget* w)
{
    if (debugTags_) delete debugTags_;
    QStringList headerLabels = {"Debug Tag", "Terminal", "LogFile"};
    debugTags_ = new QTableWidget(w);
    debugTags_->setRowCount(tagSize);
    debugTags_->setColumnCount(3);
    debugTags_->setHorizontalHeaderLabels(headerLabels);
    debugTags_->resize(dbgTableWidth, dbgTableHigh);
    for (uint32_t i=0; i<tagSize; i++)
    {
        QTableWidgetItem *nameItem = new QTableWidgetItem(darray[i].name);
        QTableWidgetItem *tFlag = new QTableWidgetItem();
        QTableWidgetItem *lFlag = new QTableWidgetItem();
        tFlag->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        tFlag->setCheckState(darray[i].terminal ? Qt::Checked : Qt::Unchecked);

        lFlag->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        lFlag->setCheckState(darray[i].logfile ? Qt::Checked : Qt::Unchecked);

        debugTags_->setItem(i, 0, nameItem);
        debugTags_->setItem(i, 1, tFlag);
        debugTags_->setItem(i, 2, lFlag);
    }
    connect(debugTags_, &QTableWidget::cellChanged, this, &MainWindow::debugSelected);
}
