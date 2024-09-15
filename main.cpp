#include "shapetransformer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ShapeTransformer w;
    w.show();
    return a.exec();
}
