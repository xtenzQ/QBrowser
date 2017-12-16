#include "mymainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(myresources);

    QTranslator myappTranslator;
    myappTranslator.load(QApplication::applicationDirPath()+
                         QDir::separator()+"lang_ru.qm");
    a.installTranslator(&myappTranslator);

    MyMainWindow mainWidget;
    mainWidget.show();

    return a.exec();
}
