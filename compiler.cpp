#include "compiler.h"
#include <QDebug>
#include "commandtypes/var.h"
#include "commandtypes/plot.h"
#include "commandtypes/print.h"
#include "commandtypes/struct.h"
#include "commandtypes/tracemap.h"

QList<CommandInterface *> Compiler::getBuild() const
{
    return build;
}

bool Compiler::isLastBuildSuccessfull() const
{
    return lastBuildSuccessfull;
}

void Compiler::compile(QString str)
{
    build.clear();
    lastBuildSuccessfull = false;
    auto exprlist = str.split("\n");
    for ( int i = 0; i < exprlist.size(); i++) {
        if (exprlist[i].contains("\t")) {
            if(i != 0) {
                exprlist[i-1] += " " + exprlist[i].remove(0,1) + " | ";
                exprlist.erase(exprlist.begin() + i);
                i--;
            }
        }
    }
    int row = 0;
    try {
        for(auto expr : exprlist) {
            if (!expr.isEmpty()) {
                build.append(compileExpression(expr));
            }
            row++;
        }
        lastBuildSuccessfull = true;
        emit showMsg("Информация", "Билд был завершен успешно.");

    } catch (CompileException & e) {
        eh.handleError(e, row);
        emit showMsg("Ошибка", "Билд был завершен ошибкой.");
    }
}

void Compiler::getBuild()
{
    emit sendBuild(lastBuildSuccessfull, build);
}

CommandInterface * Compiler::compileExpression(QString expr)
{
    auto splittedexpr = expr.split(" ");
    if(splittedexpr.first() == "данные")
        return compileVarExpression(expr);
    else if(splittedexpr.first()  == "тренировка")
        return compileStructExpression(expr);
    else return compileFuncExpression(expr);

}

CommandInterface *Compiler::compileVarExpression(QString expr)
{
    QString name;
    QList<double> values;
    auto list = expr.split(" ", QString::SkipEmptyParts);
    auto iter = list.begin();
    if(*iter == "данные") {
        name = *++iter;
        if (*++iter == "=") {
            ++iter;
            while (iter != list.end()) {
                double val = (*iter).toDouble();
                if(val == 0.0) {
                    QStringList elist;
                    elist.append(*(iter-1));
                    elist.append(*iter);
                    elist.append((iter + 1) != list.end() ? *(iter + 1) : "");
                    elist.append("Аргумент должен быть числом:");
                    throw CompileException(elist);
                }
                values.append(val);
                iter++;
            }
        }
        else {
            QStringList elist;
            elist.append(name);
            elist.append(*iter);
            elist.append((iter + 1) != list.end() ? *(iter + 1) : "");
            elist.append("Ошибка в объявлении переменной (usage: данные ДАНН = аргументы):");
            throw CompileException(elist);
        }
    }
    Var * var = new Var(name, values);
    knownNames.append(name);
    return var;
}

CommandInterface *Compiler::compileStructExpression(QString expr)
{
    QString name;
    QVector<Var> vars;
    auto list = expr.split(" ", QString::SkipEmptyParts);
    auto iter = list.begin() ;
    if(*iter == "тренировка") {
        name = *++iter;
        if (*++iter == ":") {
            while(iter != list.end()) {
                if(iter + 1 == list.end())
                    break;
                QString varName = *++iter;
                QList<double> values;
                iter++;
                while (iter != list.end()) {
                    if(*iter == "|") {
                        vars.append(Var(varName, values));
                        break;
                    }
                    values.append((*iter).toDouble());
                    iter++;
                }
            }
        }
        else {
            QStringList elist;
            elist.append(name);
            elist.append(*iter);
            elist.append((iter + 1) != list.end() ? *(iter + 1) : "");
            elist.append("Ошибка в объявлении тренировки (usage: тренировка ТРЕН : аргументы):");
            throw CompileException(elist);
        }
    }
    Struct * struc = new Struct(name, vars);
    knownNames.append(name);
    return struc;
}

CommandInterface *Compiler::compileFuncExpression(QString expr)
{
    expr = expr.replace(" и ", ",");
    QRegExp separator("[\\(\\),]");
    auto list = split(expr, separator);
    if(list.isEmpty()) {
        list = expr.split(" ");
        QStringList elist;
        elist.append("");
        elist.append(list.first());
        elist.append((list.begin() + 1) != list.end() ? *(list.begin() + 1) : "");
        elist.append("Неизвестный идентификатор:");
        throw CompileException(elist);
    }
    if(list.first() == "значения") {
        auto args = extractArgs(list);
        if(args.isEmpty()) {
            QStringList elist;
            elist.append("");
            elist.append("значения()");
            elist.append("");
            elist.append("Функция должна принимать один или более аргументов:");
            throw CompileException(elist);
        }
        for(auto arg : args)
            if (!knownNames.contains(arg)) {
                QStringList elist;
                elist.append("");
                elist.append(arg);
                elist.append("");
                elist.append("Данное имя не определено:");
                throw CompileException(elist);
            }
        Print * print = new Print(args);
        return print;
    }
    else if(list.first() == "график") {
        auto args = extractArgs(list);
        if(args.isEmpty()) {
            QStringList elist;
            elist.append("");
            elist.append("график()");
            elist.append("");
            elist.append("Функция должна принимать один или более аргументов:");
            throw CompileException(elist);
        }
        for(auto arg : args)
            if (!knownNames.contains(arg)) {
                QStringList elist;
                elist.append("");
                elist.append(arg);
                elist.append("");
                elist.append("Данное имя не определено:");
                throw CompileException(elist);
            }
        Plot * plot = new Plot(args);
        return plot;
    }
    else if(list.first() == "анализировать") {
        auto args = extractArgs(list);
        if(args.length() > 1) {
            QStringList elist;
            elist.append("");
            elist.append("анализировать()");
            elist.append("");
            elist.append("Функция должна принимать только один аргумент:");
            throw CompileException(elist);
        }
        for(auto arg : args)
            if (!knownNames.contains(arg)) {
                QStringList elist;
                elist.append("");
                elist.append(arg);
                elist.append("");
                elist.append("Данное имя не определено:");
                throw CompileException(elist);
            }
        TraceMap * tm = new TraceMap(args);
        return tm;
    }
    else {
        QStringList elist;
        elist.append("");
        elist.append(list.first());
        elist.append((list.begin() + 1) != list.end() ? *(list.begin() + 1) : "");
        elist.append("Неизвестный идентификатор функции");
        throw CompileException(elist);
    }
}

QStringList Compiler::split(QString str, QRegExp regexp)
{
    str.replace( " ", "" );
    QStringList list;
    int mem = 0;
    for(int i = 0; i<str.size(); ++i) {
        if(i == str.indexOf(regexp, i)) {
            list.append(str.mid(mem, i-mem));
            list.append(str.mid(i, 1));               // append separator
            mem = i+1;
        }
    }
    list.removeAll("");
    return list;
}

QStringList Compiler::extractArgs(QStringList list)
{
    list.removeAt(0);
    if (list[0] == "(")
        list.removeAt(0);
    QStringList list2;
    for(int i = 0; i < list.size()-1; i+=2) {
        if (isKnownName(list[i]))
            list2.append(list[i]);
        if (list[i+1] == ",")
            continue;
        else if (list[i+1] == ")") {
            break;
        }
    }
    return list2;
}

bool Compiler::isKnownName(QString name)
{
    auto it = std::find(knownNames.begin(), knownNames.end(), name);
    if (it != knownNames.end())
        return true;
    else
        return false;
}

Compiler::Compiler()
{
    lastBuildSuccessfull = 0;
}
