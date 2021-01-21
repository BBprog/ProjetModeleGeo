#ifndef COURBEBEZIER_H
#define COURBEBEZIER_H

#include "courbeparametrique.h"
#include "point.h"

#include <QVector>

/**
 * \class   CourbeBezier courbebezier.h
 *
 * \brief   Définit une courbe de Bezier avec un ensemble de point
 */

class CourbeBezier : public CourbeParametrique
{    
public:
    CourbeBezier(QVector<Point*> points);
    ~CourbeBezier();

    Point discr(float t);

private:
    int fact(int n);
    float bernstein(int i, int n, float t);
};

#endif // COURBEBEZIER_H
