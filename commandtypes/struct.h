#ifndef STRUCT_H
#define STRUCT_H
#include "commandinterface.h"
#include "var.h"

class Struct : public CommandInterface
{
private:
    QVector<Var> vars;
    QString name;
public:
    Struct() {}
    Struct(QString name, QVector<Var> vars) : name(name), vars(vars) {}

    void accept(Executor &e) {
        e.execute(*this);
    }
    QVector<Var> getVars() const {
        return vars;
    }
    QString getName() const {
        return name;
    }
};

#endif // STRUCT_H

