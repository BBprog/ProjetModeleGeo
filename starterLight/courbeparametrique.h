#ifndef COURBEPARAMETRIQUE_H
#define COURBEPARAMETRIQUE_H

#include "point.h"
#include "segment.h"

#include <QVector>
#include <QList>
#include <QtMath>

/**
 * \class   CourbeParametrique courbeparametrique.h
 *
 * \brief   Classe mère définissant l'ensemble des courbes paramétriques
 * \details Une courbe parametrique est définit par une liste de points de contrôle.
 */

class CourbeParametrique
{
protected:
    QVector<Point*> liste_points;

public:
    CourbeParametrique();
    ~CourbeParametrique();

    void rotate(float angle);
    CourbeParametrique rotateCurve(float angle);

    QVector<Point*> getPoints();
    QList<Segment> getSegments();

    int getSize();
    Point* get(int index);
    Point* getStart();
    Point* getEnd();

    CourbeParametrique &operator =(const CourbeParametrique &c);
};

#endif // COURBEPARAMETRIQUE_H
