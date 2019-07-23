#include <QtCore>
#include <QMetaObject>
#include <QMetaProperty>
#include "dqmodel.h"
#include "dqmetainfoquery_p.h"
#include "dqlist.h"

#include "dqsql.h"

//#define TABLE_NAME "Model without DQ_MODEL"
#define TABLE_NAME ""
#define PRIMARY_KEY_NAME "id"


DQModel::DQModel() : m_connection ( DQConnection::defaultConnection()){
    m_action = NO_ACTION;
}

DQModel::DQModel(DQConnection connection) : m_connection(connection)
{
    m_action = NO_ACTION;
}

DQModel::~DQModel(){
}

void DQModel::setPrimaryKeyName(const QString keyName)
{
    id.setPrimaryKeyName(keyName);
}

QString DQModel::tableName() const{
    return TABLE_NAME;
}

QString DQModel::TableName() {
    return TABLE_NAME;
}

void DQModel::setConnection(DQConnection connection){
    m_connection = connection;
}

DQConnection DQModel::connection(){
    return m_connection;
}

bool DQModel::insert() {
    if (!clean() ) {
        qDebug() << "model not clean";
        return false;
    }
    DQModelMetaInfo *info = metaInfo();
    Q_ASSERT(info);

    QStringList fields = info->fieldNameList();
    QStringList nonNullFields;


    foreach (QString field , fields) {
        QVariant v = info->value(this,field);
//        if ( field == id.primaryKeyName() ) // skip id field when forceInsert
//            continue;

        if (!v.isNull() ) {
            //  qDebug() << field;
            nonNullFields << field;
        }
    }

    bool res ;

    DQSql sql = m_connection.sql();
    res = sql.insertInto(info,this,nonNullFields,false);
    //QMessageBox::warning(0,"Error","Primary key is null, please set var \"id\"");

    m_connection.setLastQuery(sql.lastQuery());
    if(!res){
        lastError = m_connection.lastQuery().lastError().text();
    }else{
        //qDebug() << "chay duoc nek " << m_connection.lastQuery().lastQuery();
    }

    return res;
}

bool DQModel::update(const QString primaryKeyValue)
{
    if (!clean() ) {
        qDebug() << "model not clean";
        return false;
    }
    DQModelMetaInfo *info = metaInfo();
    Q_ASSERT(info);

    QStringList fields = info->fieldNameList();
    QStringList nonNullFields;


    foreach (QString field , fields) {
        QVariant v = info->value(this,field);
        if ( field == id.primaryKeyName() ) // skip id field when forceInsert
            continue;

        if (!v.isNull() ) {
            //  qDebug() << field;
            nonNullFields << field;
        }
    }

    bool res ;

    DQSql sql = m_connection.sql();
    res = sql.replaceInto(info,this,nonNullFields,id.primaryKeyName()+"='"+primaryKeyValue+"'");
    //QMessageBox::warning(0,"Error","Primary key is null, please set var \"id\"");

    m_connection.setLastQuery(sql.lastQuery());
    if(!res){
        lastError = m_connection.lastQuery().lastError().text();
    }else{
        //qDebug() << "chay upadte " << m_connection.lastQuery();
    }

    return res;
}

bool DQModel::update(DQWhere clause)
{
    if (!clean() ) {
        qDebug() << "model not clean";
        return false;
    }
    DQModelMetaInfo *info = metaInfo();
    Q_ASSERT(info);

    QStringList fields = info->fieldNameList();
    QStringList nonNullFields;


    foreach (QString field , fields) {
        QVariant v = info->value(this,field);
        if ( field == id.primaryKeyName() ) // skip id field when forceInsert
            continue;
        if (!v.isNull() ) {
            //  qDebug() << field;
            nonNullFields << field;
        }
    }

    bool res ;

    DQSql sql = m_connection.sql();
    res = sql.replaceInto(info,this,nonNullFields,clause.toString());

    m_connection.setLastQuery(sql.lastQuery());
    if(!res){
        lastError = m_connection.lastQuery().lastError().text();
    }else{
        //qDebug() << "chay upadte voi dqclause " << m_connection.lastQuery().lastQuery() << clause.toString();
    }

    return res;
}

bool DQModel::clear()
{
    QStringList list = metaInfo()->fieldNameList();
    foreach (const QString &key, list) {
        metaInfo()->setValue(this,key,QVariant());
    }
    return true;//I dont
}

bool DQModel::autoCRUD()
{
    if(m_where.isNull()){
        qDebug() << "Can't using autoCRUD with 'where isNull'";
        return false;
    }
    switch (m_action) {
    case UPDATE:
        qDebug() << m_where.toString();
        if(this->update(m_where)){

            this->setAction(NO_ACTION);
            return true;
        }
        else{
            return false;
        }
        break;
    case INSERT:
        if(this->insert()){
            this->setAction(NO_ACTION);
            return true;
        }else{
            return false;
        }
        break;
    case DELETE_EXEC:
        if(this->remove(m_where)){
            this->setAction(NO_ACTION);
            return true;
        }else{
            qDebug() << "Remove success";
            return true;
        }
        break;
    case NO_ACTION:
        return true;
    default:
        qDebug() << "No action  ";
        break;
    }
    return false;
}

bool DQModel::load(DQWhere where){
    bool res = false;

    _DQMetaInfoQuery query( metaInfo() ,  m_connection);

    query = query.filter(where).limit(1);
    res = query.exec();
    if (res){
        res = query.next();
        if (res){
            //qDebug() << query.
            res = query.recordTo(this);
        }else{
            lastError = "return no result, check where clause " + where.toString();
        }
    }else{
        lastError = query.lastQuery().lastQuery();
    }

    if (!res)
        id->clear();

    m_connection.setLastQuery(query.lastQuery());
    return res;
}

bool DQModel::load()
{
    bool res = false;

    _DQMetaInfoQuery query( metaInfo() ,  m_connection);
    DQWhere where(id.primaryKeyName(),QStringLiteral("="),this->primaryKeyValue());
    query = query.filter(where).limit(1);
    res = query.exec();
    if (res){
        res = query.next();
        if (res){
            res = query.recordTo(this);
        }else{
            lastError = "return no result, check where clause " + where.toString();
        }
    }else{
        lastError = query.lastQuery().lastQuery();
    }

    if (!res)
        id->clear();

    m_connection.setLastQuery(query.lastQuery());
    return res;
}

bool DQModel::remove(const QVariant &primaryKeyValue)
{
    _DQMetaInfoQuery query( metaInfo() ,  m_connection);

    query = query.filter(DQWhere(id.primaryKeyName(),QStringLiteral(" = "),primaryKeyValue) );
    bool res = query.remove();
    if (res){
        id->clear();
    }else{
        lastError = query.lastQuery().lastError().text();
        qDebug()<< lastError;
    }

    m_connection.setLastQuery( query.lastQuery());

    return res;
}

bool DQModel::remove(DQWhere where) {

    _DQMetaInfoQuery query( metaInfo() ,  m_connection);


    query = query.filter(where);
    bool res = query.remove();
    if (res){
        id->clear();
    }else{
        lastError = query.lastQuery().lastError().text();
        qDebug()<< lastError;
    }

    m_connection.setLastQuery( query.lastQuery());

    return res;
}
///
/// \brief DQModel::primaryKeyValue
/// \return
///
const QVariant DQModel::primaryKeyValue()
{
    //find primaryKey in field lists; and return
    return metaInfo()->value(this,id.primaryKeyName());
}

void DQModel::setPrimaryKeyValue(const QVariant &value)
{
    if(!metaInfo()->setPrimaryKeyValue(this,id.primaryKeyName(),value)){
        qDebug() << "Can't set primarykey value";
    }

}

bool DQModel::clean(){
    return true;
}

DQSharedList DQModel::initialData() const {
    return DQSharedList();
}

DQModel::FlagsAction DQModel::action() const
{
    return m_action;
}

void DQModel::setAction(const FlagsAction &action)
{
    m_action = action;
}

DQWhere DQModel::where() const
{
    return m_where;
}

void DQModel::setWhere(const DQWhere &where)
{
    m_where = where;
}
