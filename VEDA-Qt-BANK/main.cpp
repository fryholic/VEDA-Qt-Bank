#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <QFontDatabase>
#include "bankmodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 앱 정보 설정
    app.setApplicationName("한화 가상 은행");
    app.setOrganizationName("Hanwha");
    app.setOrganizationDomain("hanwha.com");
    app.setWindowIcon(QIcon(":/images/bank_logo.png"));

    // 폰트 로드
    QFontDatabase::addApplicationFont(":/fonts/05HanwhaGothicR.ttf");
    QFontDatabase::addApplicationFont(":/fonts/04HanwhaGothicB.ttf");

    // 앱 스타일 설정
    app.setStyleSheet(
        "QWidget { background-color: #F8F9FA; color: #000000; font-family: 'Noto Sans KR'; }"
        "QPushButton { background-color: #F3732B; color: white; border-radius: 4px; padding: 8px 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #F89B6C; }"
        "QPushButton:pressed { background-color: #FBB584; }"
        "QLineEdit { border: 1px solid #CCCCCC; border-radius: 4px; padding: 8px; background-color: white; }"
        "QLabel { color: #000000; }"
        );

    BankModel bankModel;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("bankModel", &bankModel);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
