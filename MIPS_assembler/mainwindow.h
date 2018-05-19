#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QWidget>
#include <QDebug>
#include <QFile>
#include <QRegExp>
#include <QVector>
#include <QDialog>
#include <QPlainTextEdit>
#include <QString>
#include <QStringList>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QTextCodec>
#include <QMessageBox>
#include <QCharRef>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_textASM_textChanged();

    void on_open_file_triggered();

    void on_toBin_triggered();

    void on_exit_triggered();

    void on_toCoe_triggered();

    void on_saveas_triggered();

    void on_save_triggered();

    void on_disasm_triggered();


    void on_help_triggered();

    void on_aboutIt_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
