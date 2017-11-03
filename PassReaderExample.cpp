#include "passReader.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PassReader pr;
    pr.readPass();

    return a.exec();
}


