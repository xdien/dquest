/**
	@author Ben Lau
 */

#ifndef DQSQLITEENGINE_H
#define DQSQLITEENGINE_H

#include "dqmodelmetainfo.h"
#include "dqengine.h"
#include "dqsql.h"
#include "dqsqlstatement.h"

/// Sqlite database engine

class DQSqliteEngine : public DQEngine
{
public:
    DQSqliteEngine();

    virtual QString name();

    virtual bool open(QSqlDatabase  db);

    virtual bool isOpen() const;

    virtual void close();

    /// Add a model to the engine
    virtual bool addModel(DQModelMetaInfo* info);

    virtual QList<DQModelMetaInfo*> modelList() const;

    /// Create the model on database if it is not existed
    /**
      @remarks It don't verify the schema.
      @return TRUE if the model is created successfully or it is already created
     */
    virtual bool createModel(DQModelMetaInfo* info);

    /// Drop the model from database
    virtual bool dropModel(DQModelMetaInfo* info);

    /// RETURN TRUE if the model is existed in the database.
    virtual bool existsModel(DQModelMetaInfo* info);

    virtual bool update(DQAbstractModel* model, QStringList fields = QStringList(), bool forceInsert = false);

    /// Create index
    virtual bool createIndex(const DQBaseIndex &index);

    /// Drop the index
    virtual void dropIndex(QString name);

    /// Get the assoicated DQSql instance
    virtual DQSql sql();

private:
    QMutex mutex;
    DQSql m_sql;

    QList<DQModelMetaInfo*> m_models;

};

#endif // DQSQLITEENGINE_H