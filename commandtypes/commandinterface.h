#ifndef COMMANDINTERFACE_H
#define COMMANDINTERFACE_H

#include "executor.h"

class CommandInterface {
public:
  virtual void accept(Executor &e) = 0;

  virtual ~CommandInterface() = default;
};


#endif // COMMANDINTERFACE_H
