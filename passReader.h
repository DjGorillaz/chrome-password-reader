#pragma once

#include <windows.h>
#include <memory>
#include <iostream>

#include <QCoreApplication>
#include <QObject>
#include <QTimer>
#include <QDir>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QTextStream>

class PassReader : public QObject
{
    Q_OBJECT
public:
    explicit PassReader(QObject* parent, const QString& outputFile);
    ~PassReader();

public slots:
    void readPass();

signals:
    void finished();

private:
    QString outputFile;
    std::unique_ptr<QTimer> timer;
    std::unique_ptr<QSqlDatabase> db;
};