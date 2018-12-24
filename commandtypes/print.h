#ifndef PRINT_H
#define PRINT_H
#include "commandinterface.h"

#include <QVector>
#include <QString>

class Print :public CommandInterface
{
private:
    QStringList argNames;
public:
    Print(QStringList args) : argNames(args) {}

    void accept(Executor &e) {
        e.execute(*this);
    }
    QStringList getArgNames() const {
        return argNames;
    }
};


#endif // PRINT_H
