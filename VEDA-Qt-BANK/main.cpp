#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 앱 정보 설정
    app.setApplicationName("VEDA");
    app.setWindowIcon(QIcon(":/images/bank_logo.png"));
    
    // 폰트 로드
    QFontDatabase::addApplicationFont(":/fonts/05HanwhaGothicEL.ttf");
    QFontDatabase::addApplicationFont(":/fonts/06HanwhaGothicL.ttf");

    QFontDatabase::addApplicationFont(":/fonts/03HanwhaL.ttf");
    
    // 스타일시트 설정
    QFile styleFile(":/styles/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        app.setStyleSheet(styleSheet);
        styleFile.close();
    }
    
    MainWindow w;
    w.show();
    
    return app.exec();
}
