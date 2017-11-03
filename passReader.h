#ifndef PASSREADER_H
#define PASSREADER_H

#include <QCoreApplication>
#include <QTimer>
#include <QDir>
#include <QtSql/QSqlDatabase>

class PassReader : public QObject
{
    Q_OBJECT
public:
    explicit PassReader(QObject* parent = 0, const QString& defaultPath = QDir::currentPath());
    ~PassReader();

public slots:
    bool const readPass();

signals:
    void fileSaved(QString filePath);

private:
    QString path;
    QTimer* timer;
    QSqlDatabase* db;
};

#endif // PASSREADER_H
