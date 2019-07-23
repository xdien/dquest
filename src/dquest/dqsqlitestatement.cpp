#include <QStringList>
#include <QtCore>
#include "dqsqlitestatement.h"
#include <QSqlDriver>
#include <QSqlField>

DQSqliteStatement::DQSqliteStatement()
{
}

QString DQSqliteStatement::_createTableIfNotExists(DQModelMetaInfo *info) {
    QString statement = QStringLiteral("%1 (\n%2\n);");
    QString createTable = QStringLiteral("CREATE TABLE IF NOT EXISTS %1 ");

    QStringList columnDefList;

    int n = info->size();

    for (int i = 0 ; i < n;i++){
        const DQModelMetaInfoField *f = info->at(i);
        QString typeName = columnTypeName(f->type);

        if (typeName.isNull()) {
            qWarning() << QStringLiteral("%1::%3 - DQField<%2> is not supported yet")
                        .arg(info->name()).arg(QVariant::typeToName(f->type)).arg(f->name);
            continue;
        }

        QString columnDef = QStringLiteral("%1 %2 %3")
                            .arg(f->name)
                            .arg(columnTypeName(f->type))
                            .arg(columnConstraint(f->clause) );
        columnDefList << columnDef;
    }

    QList<DQModelMetaInfoField> foreignKeyList = info->foreignKeyList();
    n = foreignKeyList.size();

    for (int i = 0; i < n ;i++){
        DQModelMetaInfoField f = foreignKeyList.at(i);
        QVariant v = f.clause.flag(DQClause::FOREIGN_KEY);
        DQModelMetaInfo * targetInfo = (DQModelMetaInfo*) v.value<void *>();
        Q_ASSERT(targetInfo);

        QString columnDef = QStringLiteral("FOREIGN KEY(%1) REFERENCES %2(id)")
                            .arg(f.name)
                            .arg(targetInfo->name() );

        columnDefList << columnDef;
    }

    QString sql;
    sql = statement
          .arg(createTable.arg(info->name()))
          .arg(columnDefList.join(QStringLiteral(",\n")));

    return sql;
}

QString DQSqliteStatement::columnTypeName(QVariant::Type type) {
    QString res;
    switch (type){
    case QVariant::Int:
    case QVariant::UInt:
    case QVariant::LongLong:
    case QVariant::ULongLong:
        res = QStringLiteral("INTEGER");
        break;
    case QVariant::Double:
        res = QStringLiteral("DOUBLE");
        break;
    case QVariant::String:
    case QVariant::StringList:
        res = QStringLiteral("TEXT");
        break;
    case QVariant::DateTime:
        res = QStringLiteral("DATETIME");
        break;
    case QVariant::Date:
        res = QStringLiteral("DATE");
        break;
    case QVariant::ByteArray:
        res = QStringLiteral("BLOB");
        break;
    case QVariant::Bool:
        res = QStringLiteral("BOOLEAN");
        break;
    default:
        break;
    }
    return res;
}

QString DQSqliteStatement::columnConstraint(DQClause clause){
    QStringList res;
    if (clause.testFlag(DQClause::NOT_NULL)) {
        res << QStringLiteral("NOT NULL");
    }

    if (clause.testFlag(DQClause::UNIQUE)) {
        res << QStringLiteral("UNIQUE");
    }

    if (clause.testFlag(DQClause::DEFAULT)) {
        QVariant value = clause.flag(DQClause::DEFAULT);
//        QString fvalue = formatValue(value,true); // User should format the value by themself.
        QString fvalue = value.toString();

        res << QStringLiteral("DEFAULT %1 ")
                .arg(fvalue);
    }

    if (clause.testFlag(DQClause::PRIMARY_KEY)) {
        res << QStringLiteral("PRIMARY KEY AUTOINCREMENT");
    }

    return res.join(QStringLiteral(" "));
}

QString DQSqliteStatement::driverName(){
    return QStringLiteral("SQLITE");
}

QString DQSqliteStatement::exists(DQModelMetaInfo *info) {
    return QStringLiteral("SELECT name FROM sqlite_master WHERE type='table' and name ='%1'").arg(info->name());
}
