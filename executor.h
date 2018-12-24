#ifndef EXECUTOR_H
#define EXECUTOR_H
#include <QStack>
#include <QObject>

class CommandInterface;
class Plot;
class Print;
class Struct;
class TraceMap;
class Var;

class Executor : public QObject
{
    Q_OBJECT
private:
    QList<Var> * varStack;
    QList<Struct> * structStack;

    Var findInVarStack(QString varName) const;
public:
    Executor();
    ~Executor();
    void execute(Plot &ref);
    void execute(Print &ref);
    void execute(Struct &ref);
    void execute(TraceMap &ref);
    void execute(Var &ref);
public slots:
    void run();

    void runBuild(bool, QList<CommandInterface *>);
signals:
    void printVar(QString varName, QList<double> values);

    void openPlot(QString plotName);
    void addVarToPlot(QString plotName, QString lineName, QList<double> values);
    void closePlot(QString plotName);

    void openTraceMap(QString trenName, int cals, int dist);
    void addPointToMap(double SYS, double DIA, double PUL, double TEMP, double N, double E);
    void closeTraceMAp();

    void getBuild();

    void showMsg(QString type, QString what);

};

#endif // EXECUTOR_H
