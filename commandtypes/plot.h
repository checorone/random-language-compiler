#ifndef PLOT_H
#define PLOT_H
#include "commandinterface.h"
#include <QVector>
#include <QString>

class Plot : public CommandInterface
{
private:
    QStringList argNames;
    QString name;
public:
    Plot(QStringList args) : argNames(args) {}

    void accept(Executor &e) {
        e.execute(*this);
    }
    QStringList getArgNames() const {
        return argNames;
    }
    QString getName() const {
        return name;
    }
};


#endif // PLOT_H
