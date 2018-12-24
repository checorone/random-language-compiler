#ifndef VAR_H
#define VAR_H
#include "commandinterface.h"


class Var : public CommandInterface
{
private:
    QString name;
    QString refName;
    QList<double> values;
public:
    Var() {}
    Var(QString ref) : refName(ref) {}
    Var(QString name, QList<double> values) : name(name), values(values) {}
    void accept(Executor &e) {
        e.execute(*this);
    }

    QString getName() const {
        return name;
    }

    QList<double> getValues() const {
        return values;
    }

    QString getRefName() const {
        return refName;
    }
};


#endif // VAR_H
