#include "extensions.h"

#include <QApplication>
#include <QScreen>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSettings settings("cn1ext-config.ini", QSettings::IniFormat);
    settings.beginGroup("General");
    bool checked=settings.value("ThemeFusion").toBool();
    settings.endGroup();

    if(checked==true){
        a.setStyle("fusion");
    }

    Extensions w;

    QScreen *screen=a.primaryScreen();
    QRect dimensionsEcran=screen->geometry();
    int x = dimensionsEcran.width() /2 - w.frameSize().width()/2;
    int y = (dimensionsEcran.height() /2)-30 - w.frameSize().height()/2;
    w.move(x,y);

    w.show();

    return a.exec();
}
