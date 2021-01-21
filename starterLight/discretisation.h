#ifndef DISCRETISATION_H
#define DISCRETISATION_H

#include "segment.h"
#include "point.h"
#include "courbeparametrique.h"

#include <cmath>
#include <QVector>

/**
 * \class   Discretisation discretisation.h
 *
 * \brief   Classe chargée de discrétiser un carreau de Bezier
 * \details Elle stocke une liste de \e CourbeParametrique et un nombre
 *          d'élément à discretiser par courbe.
 *          On peut appliquer une discrétisation sur ces courbes pour obtenir un carreau de Bezier.
 *          La classe gère uniquement les courbe de Bezier.
 */

class Discretisation
{
public:
    Discretisation();
    ~Discretisation();

    void setN(int n);
    void addCurve(CourbeParametrique c);
    void set(QList<CourbeParametrique> c, int n = 2);

    void discr();
    Point discr_step(float u, float v);

    int getN();
    int getSize();
    QVector<Point> getPoints();
    QList<CourbeParametrique> getCarreau();

private:
    QList<CourbeParametrique> m_curves;
    QVector<Point> pts;
    int nb_elements = 1;

    int fact(int n);
    float bernstein(int i, int n, float t);
};

#endif // DISCRETISATION_H
