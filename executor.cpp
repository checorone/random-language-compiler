#include "executor.h"
#include "commandtypes/var.h"
#include "commandtypes/plot.h"
#include "commandtypes/print.h"
#include "commandtypes/struct.h"
#include "commandtypes/tracemap.h"
#include <QMap>
#include <QDebug>
#include <cmath>

Var Executor::findInVarStack(QString varName) const
{
    for(auto var : *varStack)
        if(var.getName() == varName)
            return var;
    return Var();
}


Executor::Executor()
{
    varStack = new QList<Var>();
    structStack = new QList<Struct>();
}

Executor::~Executor()
{
    delete varStack;
    delete structStack;
}

void Executor::execute(Plot &ref)
{
    emit openPlot(ref.getName());
    auto args = ref.getArgNames();
    for (auto varName : args) {
        auto var = findInVarStack(varName);
        if (var.getName() != "") {
            emit addVarToPlot(ref.getName(), var.getName(), var.getValues());
        }
    }
    emit closePlot(ref.getName());
}

void Executor::execute(Print &ref)
{
    auto args = ref.getArgNames();
    for (auto varName : args) {
        auto var = findInVarStack(varName);
        if (var.getName() != "") {
             emit printVar(var.getName(), var.getValues());
        }
    }
}

void Executor::execute(Struct &ref)
{
    structStack->append(ref);
}

double measure(double lat1, double lon1, double lat2, double lon2){  // generally used geo measurement function
    double R = 6378.137; // Radius of earth in KM
    double dLat = lat2 * M_PI / 180 - lat1 * M_PI / 180;
    double dLon = lon2 * M_PI / 180 - lon1 * M_PI / 180;
    double a = sin(dLat/2) * sin(dLat/2) +
        cos(lat1 * M_PI / 180) * cos(lat2 * M_PI/ 180) *
        sin(dLon/2) * sin(dLon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    double d = R * c;
    return d / 10; // meters
}

void Executor::execute(TraceMap &ref)
{
    QMap<QString, QList<double>> varMap;
    auto structName = ref.getVarNames();
    Struct strct;
    for(auto var : *structStack)
        if(var.getName() == structName[0])
            strct = var;
    int argsCount = 0;
    for (auto var : strct.getVars())
        varMap.insert(QString::number(argsCount++), var.getValues());
    int size = varMap["1"].size();
    auto lat = varMap["5"];
    auto lon = varMap["6"];
    double dist = 0;
    double weight = varMap["0"][1];
    double cals = 0;
    for(int i = 0; i < size-1;i++) {
        dist += measure(lat[i], lon[i], lat[i+1], lon[i+1]);
    }
    cals = dist * weight;
    emit openTraceMap("бег", int(cals), int(dist));
    for (int i = 0; i < size; i++)
        emit addPointToMap(varMap["1"][i],varMap["2"][i],varMap["3"][i],varMap["4"][i],varMap["5"][i],varMap["6"][i]);
    emit closeTraceMAp();
}

void Executor::execute(Var &ref)
{
    if(ref.getRefName().isEmpty())
        varStack->append(ref);
    else {
        auto var = findInVarStack(ref.getRefName());
        varStack->append(Var(ref.getName(), var.getValues()));
    }
}

void Executor::run()
{
    emit getBuild();
}

void Executor::runBuild(bool successfull, QList<CommandInterface *> commands)
{
    if (successfull)
        for( auto command : commands)
            command->accept(*this);
    else
        emit showMsg("Ошибка", "Последний билд был завершен ошибкой. Запуск невозможен.");
}
