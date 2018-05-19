#ifndef MIPSASM_H
#define MIPSASM_H
#include <QString>
#include <QtGui>
#include <QMap>
#include <QChar>
#include <QPair>
#include <QVector>
#include <QRegExp> //正则表达式，检测输入
#include <QDebug>
#include <QFile>
#include <QStringList>
#include <math.h>

QStringList asm_in(QString &input_text);
QStringList scan(QStringList& input_text);
QStringList creatLabel(QString& input_text);
QString asm2bin(QString asm_code);
QString bin2asm(QString bin_code);
QString hex2bin(QString hex_code);
int bin2int(QString bin_code);
void setRegS(const QString& rs, QString& machineCode);
void setRegT(const QString& rt, QString& machineCode);
void setRegD(const QString& rd, QString& machineCode);
void setShift(const QString& shift, QString& machineCode);
void setImm(const QString& imm, QString& machineCode);
void setTarget(const QString& target, QString& machineCode);
#endif // MIPSASM_H
