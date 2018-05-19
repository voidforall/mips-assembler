#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mipsasm.h"
#include "helper.h"
#include "about.h"
extern int pc; //global program counter
extern QMap<QString, int> label;
extern QMap<int, QString> dislabel;
QString file_opened;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_textASM_textChanged()
{
    QString asmtext = ui->textASM->toPlainText();
    QStringList asm_row,asm_code;
    QString bin_code;
    QStringList bin_codelist;
    asm_row = asm_in(asmtext);
    asm_code = scan(asm_row);
    QList<QString>::iterator it = asm_code.begin();
    pc = 0;
    for(int i = 0;it!=asm_code.end();it++,i++){
        if(asm_code.length() != i+1){
            bin_code = asm2bin(asm_code.at(i));
            bin_codelist.append(bin_code + '\n');
        }
    }
    ui->textBIN->clear();
    it = bin_codelist.begin();
    for(int i = 0; it != bin_codelist.end(); it++,i++){
        ui->textBIN->textCursor().insertText(bin_codelist.at(i));
    }
}

void MainWindow::on_open_file_triggered() //打开文件并显示在编辑界面
{
    ui->textASM->clear();
    QString content;
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("Allfile(*.*);;txtfile(*.txt)"));
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << " Could not open the file for reading";
        return;
    }
    file_opened = fileName;
    //留坑 不同类型文件的显示区域不同（.asm / .coe / .bin）
    QTextStream in(&file);
    in.setCodec("utf-8"); //qtextstream默认不是utf8，会出现中文乱码
    while(!in.atEnd()){
        content = in.readLine();
        content += "\n";
        ui->textASM->insertPlainText(content);
    }
    QMessageBox::information(this,"Open file","The content in file "+fileName+" can be edited now.","OK");
}

void MainWindow::on_toBin_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,  QString::fromLocal8Bit("文件另存为"),
            "",  tr("Config Files (*.bin)"));
    if(!fileName.isNull()){
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream out(&file);
        QString bin_code;
        out.setCodec("utf-8");
        bin_code = ui->textBIN->toPlainText();
        out << bin_code;
    }
}

QString bin2Hex(QString bin_code){
    QString hex_code = "";
    QString temp;
    int sum = 0;
    for(int i = 0; i < 8; i++){
        if(bin_code.at(4*i) == '1')
            sum += 8;
        if(bin_code.at(4*i+1) == '1')
            sum += 4;
        if(bin_code.at(4*i+2) == '1')
            sum += 2;
        if(bin_code.at(4*i+3) == '1')
            sum += 1;
        temp = QString::number(sum,16);
        sum = 0;
        hex_code += temp;
    }
    return hex_code;
}

void MainWindow::on_toCoe_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,  QString::fromLocal8Bit("文件另存为"),
            "",  tr("Config Files (*.coe)"));
    if(!fileName.isNull()){
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream out(&file);
        QString bin_code;
        QStringList bin32;
        QString hex32;
        out.setCodec("utf-8");
        out << "memory_initialization_radix=16" << "\n";
        out << "memory_initialization_vector= " << "\n";
        bin_code = ui->textBIN->toPlainText().toLocal8Bit();
        bin32 = bin_code.split('\n',QString::SkipEmptyParts);
        for(int i = 0; i < bin32.size(); i++){
            hex32 = bin2Hex(bin32.at(i));
            out << hex32 << ", ";
            if(i % 10 == 9) out << "\n";
        }
    }
}

void MainWindow::on_saveas_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,  QString::fromLocal8Bit("文件另存为"),
            "",  tr("Config Files (*.asm)"));
    if(!fileName.isNull()){
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QString asm_code;
        QTextStream out(&file);
        out.setCodec("utf-8");
        asm_code = ui->textASM->toPlainText();
        out << asm_code;
    }
}

void MainWindow::on_save_triggered()
{
    QString fileName;
    if(!file_opened.isNull()){
        fileName = file_opened;
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QString asm_code;
        QTextStream out(&file);
        out.setCodec("utf-8");
        asm_code = ui->textASM->toPlainText();
        out << asm_code;
    }
}

void MainWindow::on_exit_triggered()
{
    this->close();
}

void MainWindow::on_disasm_triggered()
{
    QString content;
    QStringList bin_code;
    QString asm_code;
    pc = 0;
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("Allfile(*.*)"));
    QString savedFile = fileName.split(fileName.indexOf('.')).at(0);
    savedFile += ".asm";
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << " Could not open the file for reading";
        return;
    }
    QTextStream in(&file);
    in.setCodec("utf-8");
    content = in.readAll();
    ui->textASM->clear();
    ui->textBIN->setPlainText(content);
    QFile filesaved(savedFile);
    if (!filesaved.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&filesaved);
    out.setCodec("utf-8");
    bin_code = creatLabel(content);
    pc = 0;
    QList<QString>::Iterator it = bin_code.begin(),itend = bin_code.end();
    int i = 0;
    for (;it != itend; it++,i++){
        if(dislabel.contains(pc)){
            out << dislabel[pc] << ":" << "\n";
            ui->textASM->insertPlainText(dislabel[pc] + ":" + "\n");
        }
        asm_code = bin2asm(bin_code.at(i));
        ui->textASM->insertPlainText(asm_code + ";\n");
        out << asm_code << ";" << "\n";
    }
    qDebug() << dislabel;
    QMessageBox::information(this,"Disassembly","The content in file "+fileName+" has been disassemblyed.","OK");
}


void MainWindow::on_help_triggered()
{
    helper w;
    w.exec();
}

void MainWindow::on_aboutIt_triggered()
{
    about w;
    w.exec();
}
