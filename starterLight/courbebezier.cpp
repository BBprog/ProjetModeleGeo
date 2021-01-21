#include "courbebezier.h"

#include <cmath>
#include <QDebug>


CourbeBezier::CourbeBezier(QVector<Point*> points)
{
    this->liste_points = points;
}

CourbeBezier::~CourbeBezier()
{

}

int CourbeBezier::fact(int n)
{
  return (n == 1 || n == 0) ? 1 : fact(n - 1) * n;
}

/**
 * \brief   Calcul un polynôme de Bernstein pour un point
 * \param   i     Indice du point
 * \param   n     Nombre de point
 * \param   t     "Pas" de discrétisation
 * \return  \e float
 */
float CourbeBezier::bernstein (int i, int n, float t)
{
    return ( fact(n) / (fact(i) * fact(n-i)) * pow(t, i) * pow(1-t, n-i) );
}

/**
 * \brief   Discretise un point de la courbe de Bezier
 * \param   t     Pramètre de discrétisation
 * \return  Le \e Point de la courbe correspondant
 */
Point CourbeBezier::discr(float t)
{
    Point p;
    float x, y, z;
    x = y = z = 0;
    for (int i = 0; i < this->liste_points.size(); ++i)
    {
        x += this->liste_points.at(i)->getX() * bernstein(i, this->liste_points.size()-1, t);
        y += this->liste_points.at(i)->getY() * bernstein(i, this->liste_points.size()-1, t);
        z += this->liste_points.at(i)->getZ() * bernstein(i, this->liste_points.size()-1, t);
    }
    p.setX(x);
    p.setY(y);
    p.setZ(z);

    return p;
}
