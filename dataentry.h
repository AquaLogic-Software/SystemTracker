#ifndef DATAENTRY_H
#define DATAENTRY_H
#include <QByteArray>
#include <QList>
#include <QIODevice>
#include <QDataStream>
#include <QBuffer>
#include <QDateTime>
#include <QDebug>
#include <QUuid>

class DataEntry
{
    QString Data;
public:

    enum DataType
    {
        Type_Numerical = 0x0001,
        Type_String = 0x0002,
        Type_Kg = 0x0010,
        Type_G = 0x0020,
        Type_Lbs = 0x0030,
        Type_Oz = 0x0040,
        Type_PPM = 0x0050,
        Type_PH = 0x0060,
        Type_K = 0x0070,
        Type_C = 0x0080,
        Type_F = 0x0090,
        Type_L = 0x00A0,
        Type_Gal = 0x00B0,
        Type_In = 0x00C0,
        Type_Mm = 0x00D0
    };

    struct Descriptor
    {
        QString Name;
        QString ID;
        DataType Type;
        quint16 Flags;

        Descriptor(QString name, DataType type, QString id, quint16 flags = 0) :
            Name(name), ID(id), Type(type)
        {
            this->Flags = flags;
        }

        Descriptor()
        {

        }

        bool IsString() { return ((this->Type & Type_String) == Type_String); }
        bool IsNumerical() { return ((this->Type & Type_Numerical) == Type_Numerical); }
        bool IsType(DataType type) { return ((this->Type & 0xFFF0) == type); }

        static QString GetTypeText(Descriptor descriptor)
        {
            QString text("");

            if(descriptor.IsString())
                text = QString("String reference");
            else if(descriptor.IsType(DataEntry::Type_C))
                text = QString("Celsius (C)");
            else if(descriptor.IsType(DataEntry::Type_F))
                text = QString("Fahrenheit (F)");
            else if(descriptor.IsType(DataEntry::Type_G))
                text = QString("Grams (g)");
            else if(descriptor.IsType(DataEntry::Type_Gal))
                text = QString("Gallons");
            else if(descriptor.IsType(DataEntry::Type_In))
                text = QString("Inches");
            else if(descriptor.IsType(DataEntry::Type_K))
                text = QString("Kelvin");
            else if(descriptor.IsType(DataEntry::Type_Kg))
                text = QString("Kilograms");
            else if(descriptor.IsType(DataEntry::Type_L))
                text = QString("Liters");
            else if(descriptor.IsType(DataEntry::Type_Lbs))
                text = QString("US Pounds (Lbs)");
            else if(descriptor.IsType(DataEntry::Type_Mm))
                text = QString("Millimeter");
            else if(descriptor.IsType(DataEntry::Type_Oz))
                text = QString("US Ounces (Oz)");
            else if(descriptor.IsType(DataEntry::Type_PH))
                text = QString("pH");
            else if(descriptor.IsType(DataEntry::Type_PPM))
                text = QString("Parts Per Million (PPM)");

            return text;
        }
    };

    QDateTime Timestamp;
    const Descriptor *ListDescriptor;

    DataEntry(QDateTime timestamp = QDateTime::currentDateTime())
    {
        this->Timestamp = timestamp;
    }

    QString GetString()
    {
        return this->Data;
    }

    double GetValue()
    {
        return this->Data.toDouble();
    }

    void SetValue(QString data)
    {
        this->Data.clear();
        this->Data.append(data);
    }

    void SetValue(double data)
    {
        this->Data.clear();
        this->Data.append(QString::number(data));
    }
};

#endif // DATAENTRY_H
