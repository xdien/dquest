#include <QtCore>
#include <QSqlError>
#include <QSharedDataPointer>
#include "dqmodel.h"
#include "dqsql.h"
#include "dqsqlitestatement.h"


/* DQSql */

DQSql::DQSql(DQSqlStatement *statement)
{
    d = new DQSqlPriv();
    if (statement) {

        d->m_statement = QSharedPointer<DQSqlStatement>(statement);
    }
}

DQSql::DQSql(const DQSql& other) : d(other.d) {

}

DQSql::~DQSql(){
    //delete d;
}

DQSql& DQSql::operator=(const DQSql &rhs){
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

void DQSql::setStatement(DQSqlStatement *statement) {
    d->m_statement = QSharedPointer<DQSqlStatement>(statement);
}

DQSqlStatement* DQSql::statement() {
    return d->m_statement.data();
}

void DQSql::setDatabase(QSqlDatabase db){
    d->m_db = db;
}

QSqlDatabase DQSql::database(){
    return d->m_db;
}

bool DQSql::createTableIfNotExists(DQModelMetaInfo* info){
    QString sql = d->m_statement->createTableIfNotExists(info);

    QSqlQuery q = query();

//    d->m_lastQuery = new QSqlQuery(query());
    bool ret = q.exec(sql);
    setLastQuery(q);

    return ret;
}

QSqlQuery DQSql::query(){
    return QSqlQuery(d->m_db);
}

QSqlQuery DQSql::lastQuery(){
    return QSqlQuery(*d->m_lastQuery);
}

void DQSql::setLastQuery(QSqlQuery value)
{
#ifdef Q_OS_ANDROID
#else
    d->m_mutex.lock();
#endif
    if (d->m_lastQuery == 0) {
        d->m_lastQuery = QSharedPointer<QSqlQuery>(new QSqlQuery(query()));
    }

    if (value.isActive())
        value.finish();

    *d->m_lastQuery = value;
#ifdef Q_OS_ANDROID
#else
    d->m_mutex.unlock();
#endif
}

bool DQSql::dropTable(DQModelMetaInfo* info){
    QString sql = d->m_statement->dropTable(info);

    QSqlQuery q = query();

    bool res = q.exec(sql);

    setLastQuery(q);

    return res;

//    QString sql = d->m_statement->dropTable(info);

//    d->m_lastQuery = new QSqlQuery(query());

//    return d->m_lastQuery->exec(sql);
}

bool DQSql::createIndexIfNotExists(const DQBaseIndex &index) {
    QString sql = d->m_statement->createIndexIfNotExists(index);

    QSqlQuery q = query();
    bool res = q.exec(sql);

    setLastQuery(q);

    return res;
}

bool DQSql::dropIndexIfExists(QString name){
    QString sql = d->m_statement->dropIndexIfExists(name);

    QSqlQuery q = query();
    bool res = q.exec(sql);

    setLastQuery(q);

    return res;
}

bool DQSql::exists(DQModelMetaInfo* info){
    if (d->m_db.driverName() != "QSQLITE") {
        qWarning() << "Only QSQLITE dirver is supported.";
        return false;
    }

    QString sql = DQSqliteStatement::exists(info);
//    qDebug() << sql;
    QSqlQuery q = query();

    bool res = false;
    if (q.exec(sql)) {
        if (q.next())
            res = true;
    }

    setLastQuery(q);

    return res;
}

bool DQSql::insertInto(DQModelMetaInfo* info,DQModel *model,QStringList fields,bool updateId) {
    return insertInto(info,model,fields,updateId,false);
}

bool DQSql::replaceInto(DQModelMetaInfo* info,DQModel *model,QStringList fields,QString clause){
    QString sql;
    QSqlQuery q = query();
    sql = d->m_statement->replaceInto(info,fields,clause);
    q.prepare(sql);

    foreach (QString field , fields) {
        QVariant value;
        value = info->value(model,field,true);
//        qDebug() << "bind " << field;
        q.bindValue(":" + field , value);
    }

    bool res = false;

    if (q.exec()) {
        res = true;

    }

    setLastQuery(q);

    return res;
}

bool DQSql::insertInto(DQModelMetaInfo* info,DQModel *model,QStringList fields,bool updateId,bool replace){
QString sql;

    QSqlQuery q = query();
    if (replace){
        //sql = d->m_statement->replaceInto(info,fields);
    } else {
        sql = d->m_statement->insertInto(info,fields);
    }

    q.prepare(sql);

    foreach (QString field , fields) {
        QVariant value;
        value = info->value(model,field,true);
//        qDebug() << "bind " << field;
        q.bindValue(":" + field , value);
    }

    bool res = false;

    if (q.exec()) {
        res = true;
        if (updateId) {
            if(q.lastInsertId().isValid()){// If more than one row was touched by the insert, the behavior is undefined.
                QVariant id = q.lastInsertId();
                if (model->primaryKeyValue() != id){
                    qDebug() << model->id.primaryKeyName()<<"gia tri " <<model->id.get().toString();
                    model->setPrimaryKeyValue(id);
                }
            }else{
                qDebug() << "lastInsertId not found";
            }
        }else{
            //qDebug() << "c"
        }
    }

    setLastQuery(q);

    return res;
}

DQSqlPriv::DQSqlPriv()
{
    m_lastQuery.reset();
}
