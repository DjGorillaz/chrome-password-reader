#include "passReader.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PassReader pr1;
    pr1.readPass();


    return a.exec();
}
