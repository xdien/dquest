#ifndef DQFIELD_H
#define DQFIELD_H


#include <dqbasefield.h>
#define PRIMARY_KEY_NAME "id"
/// Database field
/**
    DQField store the value of a field in database model. The format is QVariant.
    Therefore you may assign a QVariant to DQField direclty, and you may access
    the stored QVariant by using operator-> or get() function.

    @see DQModel
    @see DQForeignKey
 */

template <typename T>
class Q_DECL_EXPORT DQField : public DQBaseField
{
public:
    /// Default constructor
    DQField(){
    }

    /// Return the type id of the field
    static QVariant::Type type(){
        return (QVariant::Type) qMetaTypeId<T>();
    }

    /// Copy the value from a QVariant object
    inline QVariant operator=(const QVariant &val){
        set(val);
        return val;
    }

    /// Compare with other DQField
    inline bool operator==(const DQField& rhs) const {
        return get() == rhs.get();
    }

    /// Compare with QVariant type
    inline bool operator==(const QVariant &rhs) const {
        return get() == rhs;
    }

    /// Compare with QVariant type
    inline bool operator!=(const QVariant &rhs) const {
        return get() != rhs;
    }

    /// Compare with its template type
    inline bool operator==(const T& t) const {
        return get() == t;
    }

    /// Compare with its template type
    inline bool operator!=(const T& t) const {
        return get() != t;
    }

    /// Compare with string type
    inline bool operator==(const char *string) const {
        return get() == QString::fromUtf8(string);
    }

    /// Compare with string type
    inline bool operator!=(const char *string) const {
        return get() != QString(string);
    }

    /// Get the value of the field
    inline QVariant get(bool convert = false) const {
        return DQBaseField::get(convert);
    }

    /// Set the value of the field
    inline bool set(QVariant value) {
        return DQBaseField::set(value);
    }

    /// Cast it to the template type
    inline operator T() const {
        QVariant v = get();
        return v.value<T>();
    }

};

class Q_DECL_EXPORT DQPrimaryKey : public DQField<QString> {
public:
    DQPrimaryKey(const QString primaryKeyName = QStringLiteral(PRIMARY_KEY_NAME));
    static DQClause clause();

    inline QVariant operator=(const QVariant &val){
        set(val);
        return val;
    }

     QString primaryKeyName() const;
     void setPrimaryKeyName(const QString &primaryKeyName);

private:
    QString m_primaryKeyName;//use for name id;
};

#endif // DQFIELD_H
