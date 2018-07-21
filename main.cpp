#include "passReader.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    if (argc < 2)
        std::cerr << "Usage: chrome-password-reader.exe output_file.csv";
    else
    {
        PassReader pr{nullptr, argv[1]};

        //Exit when reading pass will be finished
        QObject::connect(&pr, &PassReader::finished, &app, &QCoreApplication::quit);
        QTimer::singleShot(0, &pr, &PassReader::readPass);
        return app.exec();
    }
    
    app.quit();
    return 0;
}


