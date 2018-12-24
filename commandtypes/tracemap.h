#ifndef TRACEMAP_H
#define TRACEMAP_H
#include "commandinterface.h"


class TraceMap : public CommandInterface
{
private:
      QStringList varNames;
      QString structName;
public:
      TraceMap(QString structName) : structName(structName) {}
      TraceMap(QStringList varNames) : varNames(varNames) {}

    void accept(Executor &e) {
        e.execute(*this);
    }
    QStringList getVarNames() const {
        return varNames;
    }
    QString getStructName() const {
        return structName;
    }
};


#endif // TRACEMAP_H
