#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickTextDocument>
#include <QTextCodec>

#include "compiler.h"
#include "executor.h"

#include "highlighter.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    Compiler compiler;
    Executor executor;

    QObject::connect(engine.rootObjects()[0], SIGNAL(compile(QString)), &compiler, SLOT(compile(QString)));
    QObject::connect(engine.rootObjects()[0], SIGNAL(run()), &executor, SLOT(run()));

    QObject::connect(&executor, SIGNAL(getBuild()), &compiler, SLOT(getBuild()));
    QObject::connect(&compiler, SIGNAL(sendBuild(bool, QList<CommandInterface *>)), &executor, SLOT(runBuild(bool, QList<CommandInterface *>)));

    engine.rootContext()->setContextProperty("exec", &executor);
    engine.rootContext()->setContextProperty("comp", &compiler);
    engine.rootContext()->setContextProperty("handler", compiler.getHandler());


    Highlighter h(engine.rootObjects()[0]->findChild<QObject*>("input")->property("textDocument").value<QQuickTextDocument*>()->textDocument());

    return app.exec();
}
