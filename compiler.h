#ifndef COMPILER_H
#define COMPILER_H
#include <QObject>
#include <QQueue>
#include "commandtypes/commandinterface.h"
#include "errorhandler.h"

class Compiler : public QObject
{
    Q_OBJECT
private:
    QList<CommandInterface *> build;
    QStringList knownNames;
    bool lastBuildSuccessfull;

    CommandInterface * compileExpression(QString);
    CommandInterface * compileVarExpression(QString);
    CommandInterface * compileStructExpression(QString);
    CommandInterface * compileFuncExpression(QString);

    QStringList split(QString str, QRegExp regexp);
    QStringList extractArgs(QStringList);
    bool isKnownName(QString name);

    ErrorHandler eh;

public:
    Compiler();

    QList<CommandInterface *> getBuild() const;
    bool isLastBuildSuccessfull() const;

    ErrorHandler * getHandler() {
        return &eh;
    }
public slots:
    void compile(QString str);

    void getBuild();

signals:
    void sendBuild(bool, QList<CommandInterface *>);
    void showMsg(QString type, QString what);
};

#endif // COMPILER_H
