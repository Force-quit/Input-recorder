#include <QApplication>
#include <QStyleFactory>
#include "../Headers/EQInputRecorder.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    EQInputRecorder w;
    w.show();
    return a.exec();
}