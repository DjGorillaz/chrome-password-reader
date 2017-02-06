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

class PassReader : public QObject
{
    Q_OBJECT
public:
    explicit PassReader(QObject* parent = 0);
    PassReader(QObject* parent, QString& str);
    ~PassReader();

    bool readPass();

signals:
    void fileSaved(QString filePath);

private:
    QString* path;
};

#endif // PASSREADER_H
