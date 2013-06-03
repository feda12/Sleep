#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include "core.h"

QT_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Core core;

    return a.exec();
}
