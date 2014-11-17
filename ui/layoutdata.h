#ifndef LAYOUTDATA_H
#define LAYOUTDATA_H

#include <QString>
#include <QPoint>

struct LayoutData
{
    enum Policy
    {
        Horizontal_Expanding = (1 << 0),
        Horizontal_Fixed = (1 << 1),
        Vertical_Expanding = (1 << 2),
        Vertical_Fixed = (1 << 3)
    };

    double Height;
    double Width;

    int X;
    int Y;

    Policy Policies;
};

#endif // LAYOUTDATA_H
