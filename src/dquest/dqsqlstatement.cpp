#include <QStringList>

#include "dqsqlstatement.h"
#include "dqexpression.h"

DQSqlStatement::DQSqlStatement()
{
}

DQSqlStatement::~DQSqlStatement()
{

}

QString DQSqlStatement::dropTable(DQModelMetaInfo *info) {
    QString sql = QStringLiteral("drop table %1;").arg(info->name());
    return sql;
}

QString DQSqlStatement::createTableIfNotExists(DQModelMetaInfo *info){
    return _createTableIfNotExists(info);
}

QString DQSqlStatement::createIndexIfNotExists(const DQBaseIndex& index){
    QString createIndex = QStringLiteral("CREATE INDEX IF NOT EXISTS %1 on %2 (%3);");

    QString sql = createIndex.arg(index.name())
                             .arg(index.metaInfo()->name())
                             .arg(index.columnDefList().join(QStringLiteral(",")));

    return sql;
}

QString DQSqlStatement::dropIndexIfExists(QString name){
    QString createIndex = QStringLiteral("DROP INDEX IF EXISTS %1;");

    QString sql = createIndex.arg(name);

    return sql;
}

QString DQSqlStatement::insertInto(DQModelMetaInfo *info,QStringList fields){
    return _insertInto(info,QStringLiteral("INSERT"),fields);
}

QString DQSqlStatement::replaceInto(DQModelMetaInfo *info,QStringList fields,const QString &clause){//update
    return _update(info,fields,clause);
}

QString DQSqlStatement::_insertInto(DQModelMetaInfo *info ,QString type, QStringList fields){
    QString sql,format;
    QStringList values;

    format = QStringLiteral("%4 INTO %1 (%2) values (%3);");

    foreach (QString f, fields) {
        values << QStringLiteral(":") + f;
    }

    sql = format.arg(info->name(), fields.join(QStringLiteral(",")),values.join(QStringLiteral(",")) , type);

    return sql;
}

QString DQSqlStatement::_update(DQModelMetaInfo *info ,QStringList fields,QString clause)
{
    QString sql,format;
    QStringList values;

    format = QStringLiteral("UPDATE %1 SET %2 where %3;");

    foreach (QString f, fields) {
        values << f+ QStringLiteral("=:") + f;
    }

    sql = format.arg(info->name(),values.join(QStringLiteral(",")),clause);
    return sql;
}


QString DQSqlStatement::select(DQSharedQuery query) {
    DQQueryRules rules;
    rules =  query;
    QStringList sql;

    sql << selectCore(rules);
    if (rules.limit() > 0 ) {
        sql << limitAndOffset(rules.limit());
    }

    if (rules.orderBy().size() > 0) {
        sql << orderBy(rules);
    }

    sql << QStringLiteral(";");

    return sql.join(QStringLiteral(" "));
}

QString DQSqlStatement::deleteFrom(DQSharedQuery query) {
    DQQueryRules rules;
    rules =  query;
    QStringList sql;

    sql << QStringLiteral("DELETE FROM %1").arg(rules.metaInfo()->name());

    DQExpression expression = rules.expression();
    if (!expression.isNull()) {

//        sql << QString("WHERE %1").arg(where.toString());
        sql << QStringLiteral("WHERE %1").arg(expression.string());

    }

    /// @todo Implemente order by

    if (rules.limit() > 0) {
        sql << limitAndOffset(rules.limit());
    }

    sql << QStringLiteral(";");

    return sql.join(QStringLiteral(" "));
}

QString DQSqlStatement::selectCore(DQQueryRules rules){
    QStringList res;

    res << QStringLiteral("SELECT %1 %2 FROM %3").arg(QStringLiteral("ALL")).arg(selectResultColumn(rules)).arg(rules.metaInfo()->name());

    DQExpression expression = rules.expression();
    if (!expression.isNull()) {
//        res << QString("WHERE %1").arg(where.toString());
        res << QStringLiteral("WHERE %1").arg(expression.string());
    }

    return res.join(QStringLiteral(" "));
}

QString DQSqlStatement::selectResultColumn(DQQueryRules rules){
    QString res;
    QStringList fields = rules.fields();
    if (fields.size() == 0)
        res = QStringLiteral("*");
    else
        res = fields.join(QStringLiteral(","));

    QString func;
    func = rules.func();
    if (!func.isEmpty()) {
        res = QStringLiteral("%1(%2)").arg(func).arg(res);
    }
    return res;
}

QString DQSqlStatement::limitAndOffset(int limit, int offset) {
    QStringList res;
    res << QStringLiteral("LIMIT %1").arg(limit);
    if (offset > 0) {
        res << QStringLiteral("OFFSET %1").arg(offset);
    }
    return res.join(QStringLiteral(" "));
}

QString DQSqlStatement::orderBy(DQQueryRules rules){
    QStringList orderingTerms;

    orderingTerms << QStringLiteral("ORDER BY");
    orderingTerms << rules.orderBy().join(QStringLiteral(","));

    return orderingTerms.join(QStringLiteral(" "));
}

QString DQSqlStatement::formatValue(QVariant value,bool trimStrings) {
    QString res;

    switch (value.type() ){

    case QVariant::String:
    case QVariant::Char:
        res = value.toString();
        if (trimStrings)
            res = res.trimmed();
        res.replace(QLatin1Char('\''), QLatin1String("''"));
        res = QStringLiteral("'%1'").arg(res);
        break;

    default:
        // @todo Implement more data type

        res = value.toString();
        break;

    }

    return res;
}
