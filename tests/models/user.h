#ifndef USER_H
#define USER_H

/// Example model - user table

#include "dqmodel.h"

class User : public DQModel
{
    DQ_MODEL
public:
    DQField<QString> userId;
    DQField<QString> name;

    DQField<QString> passwd;

    virtual bool clean() {
        QString pw = passwd->toString();
        if (pw.size() < 8){ // passwd is too short.
            return false;
        }
        return true;
    }
};

DQ_DECLARE_MODEL( User,
                  "user",
                  DQ_FIELD(userId, DQUnique | DQNotNull),
                  DQ_FIELD(name),
                  DQ_FIELD(passwd , DQNotNull)
                  );


#endif // USER_H
