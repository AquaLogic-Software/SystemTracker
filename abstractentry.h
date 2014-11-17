#ifndef ABSTRACTENTRY_H
#define ABSTRACTENTRY_H
#include <QByteArray>
#include <QList>
#include <QIODevice>
#include <QDataStream>
#include <QBuffer>

class AbstractEntry
{
    static const quint32 serial_sep = 0xFFAFA8FF;

protected:
    QByteArray val;
    QString Name;

public:

    inline bool operator==(AbstractEntry a)
    {
        return (a.GetName() == this->Name);
    }

    QDataStream& operator<<(QDataStream& os)
    {
        os << QByteArray(this->Name.toStdString().c_str());
        os << serial_sep;
        os << this->val;
        return os;
    }

    QString GetName()
    {
        return this->Name;
    }

    QByteArray GetRawValue()
    {
        return this->val;
    }
};
#endif // ABSTRACTENTRY_H
