#ifndef PASSREADER_H
#define PASSREADER_H

#include <QCoreApplication>

#include <QFile>
#include <QDir>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QDebug> //?

#include <windows.h>

class PassReader
{
public:
    PassReader();
    PassReader(QString& str);
    ~PassReader();

    bool readPass();

signals:
    void fileSaved();

private:
    QString* path;
};

#endif // PASSREADER_H
