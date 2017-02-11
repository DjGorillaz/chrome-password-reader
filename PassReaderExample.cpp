#include "passReader.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    QThread *thread = new QThread;
    PassReader* pr1 = new PassReader;
    pr1->moveToThread(thread);

    //QObject::connect(thread, SIGNAL(started()), pr1, SLOT(readPass()));
    QObject::connect(thread, &QThread::started, pr1, &PassReader::readPass);
    QObject::connect(pr1, &PassReader::fileSaved, thread, &QThread::quit);

    thread->start();

    qDebug() << "done";


    return a.exec();
}


