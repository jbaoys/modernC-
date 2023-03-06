#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showDebugDialog();
    void debugSelected(int row, int col);

private:
    void PopulateDebugTags(QWidget* w);
    Ui::MainWindow *ui;
    QDialog *debugDialog_;
    QTableWidget *debugTags_;
};
#endif // MAINWINDOW_H
