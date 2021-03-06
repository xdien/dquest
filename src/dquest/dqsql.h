#ifndef DQSQL_H
#define DQSQL_H

#include <QExplicitlySharedDataPointer>
#include <QSqlQuery>
#include <dqmodelmetainfo.h>
#include <dqindex.h>

class DQModelMetaInfo;
class DQSqlStatement;
class DQModel;

class DQSqlStatement;

class DQSqlPriv: public QSharedData{
public:
    explicit DQSqlPriv();
    QSharedPointer<DQSqlStatement> m_statement;

    QSqlDatabase m_db;

    /// The query object used in last operation
    QSharedPointer<QSqlQuery> m_lastQuery;
//bug on adnroid mutithead
#ifdef Q_OS_ANDROID
#else
    QMutex m_mutex;
#endif

};

/// A helper class for SQL statement exeuction
/**
   DQSql provides a set of pre-defined SQL operation over the connected database.
   It should be a thread safe object (not verified).

   User are not supposed to use this class except for error checking. User
   may call lastQuery() to retreive the detailed information of last
   query. It is useful to debug SQL level error.

   @remarks The thread safe capability is not verified.
 */

class DQSql
{
public:
    /// Copy constructor
    DQSql(const DQSql&);

    /// Default constructor
    ~DQSql();

    /// Get the associated DQSqlStatement generator
    DQSqlStatement* statement();

    /// The connected database
    QSqlDatabase database();

    /// Run create table of a model
    template <typename T>
    inline bool createTableIfNotExists(){
        DQModelMetaInfo* metaInfo = dqMetaInfo<T>();
        return createTableIfNotExists(metaInfo);
    }

    /// Run create table of a model
    bool createTableIfNotExists(DQModelMetaInfo* info);

    /// Run drop table of a model
    bool dropTable(DQModelMetaInfo* info);

    /// Create index
    bool createIndexIfNotExists(const DQBaseIndex &index);

    /// Drop index
    bool dropIndexIfExists(QString name);

    /// Is the model exists on database?
    bool exists(DQModelMetaInfo* info);

    /// Insert the reocrd to the database.
    /**
      @param info The meta information of writing model
      @param model The data source
      @param fields A list of fields that should be saved
      @param updateId TRUE if the ID of the model should be updated after operation
     */
    bool insertInto(DQModelMetaInfo* info,DQModel *model,QStringList fields,bool updateId);

    /// Replace the record to the database
    /**
      @param info The meta information of writing model
      @param model The data source
      @param fields A list of fields that should be saved
      @param updateId TRUE if the ID of the model should be updated after operation
     */
    bool replaceInto(DQModelMetaInfo* info, DQModel *model, QStringList fields, QString clause);

    /// Create a query object to the connected database
    QSqlQuery query();

    /// The last query object
    QSqlQuery lastQuery();

protected:
    /**
      @param statement A instance of DQSqlStatement. The ownership will be taken.
     */

    explicit DQSql(DQSqlStatement *statement = 0);

    /// Assignment operator overloading
    DQSql& operator=(const DQSql &rhs);

    /// Set the connected database
    void setDatabase(QSqlDatabase db);

    /// Assign a SQL statement generator
    /**
      @param statement A instance of DQSqlStatement. The ownership will be taken.
     */
    void setStatement(DQSqlStatement *statement);

private:
    void setLastQuery(QSqlQuery query);

    bool insertInto(DQModelMetaInfo* info,DQModel *model,QStringList fields,bool with_id,bool replace);

    QExplicitlySharedDataPointer<DQSqlPriv> d;
    //DQSqlPriv *d;
    friend class DQConnection;
    friend class DQConnectionPriv;
};

#endif // DQSQL_H
