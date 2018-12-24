#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H
#include <QStringList>
#include <stdexcept>
#include <QObject>

class CompileException : public std::runtime_error
{
private:
    QStringList str;
public:
    CompileException(QStringList stri) : std::runtime_error(stri.last().toStdString()) {stri.removeLast(); str = stri;}
    QStringList details() {return str;}
};

class ErrorHandler : public QObject
{
    Q_OBJECT
public:
    ErrorHandler();
    ~ErrorHandler();
    void handleError(CompileException & e, int num) {
        emit showError(num + 1, 0, 0, "<b>"+QString(e.what()) + "<\b> <i>" + e.details()[0] + " " + e.details()[1] + " " + e.details()[2] + "<\i>");

    }
signals:
    void showError(int strnum, int begin, int end, QString msg);

};

#endif // ERRORHANDLER_H
