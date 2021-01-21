#include "discretisation.h"

#include <QDebug>


Discretisation::Discretisation()
{

}

Discretisation::~Discretisation()
{

}

/**
 * \brief   Initialise les paramètres pour la discretisation
 * \details Affecte une liste de courbes et un nombre d'éléments.
 * \param   curves  Liste de courbes
 * \param   n       Le nombre d'éléments à discretiser
 */
void Discretisation::set(QList<CourbeParametrique> curves, int n)
{
    this->setN(n);
    this->m_curves = curves;
}

/**
 * \brief   Initialise le paramètre \a nb_elements
 * \details Affecte un nouveau nombre d'éléments. Il doit être suppérieur ou égal à 1.
 * \param   n       Le nouveau nombre d'éléments à discretiser >= 1
 */
void Discretisation::setN(int n)
{
    if (n >= 1) this->nb_elements = n;
}

int Discretisation::fact(int n)
{
    return (n == 1 || n == 0) ? 1 : fact(n - 1) * n;
}

float Discretisation::bernstein (int i, int n, float t) {
    return ( fact(n) / (fact(i) * fact(n-i)) * pow(t, i) * pow(1-t, n-i) );
}

/**
 * \brief   Ajoute une courbe
 * \param   c   La \e CourbeParametrique à ajouter
 */
void Discretisation::addCurve(CourbeParametrique c)
{
    this->m_curves.append(c);
}

/**
 * \brief   Discrétise un point du carreau de Bezier
 * \param   u   Pramètre 1 de discrétisation
 * \param   v   Pramètre 2 de discrétisation
 * \return  Le point discrétisé
 */
Point Discretisation::discr_step(float u, float v)
{
    Point p;
    float x, y, z;
    x = y = z = 0;

    for (int i = 0; i < this->m_curves.size(); ++i)
    {
        CourbeParametrique c = this->m_curves.at(i);
        for (int j = 0; j < c.getSize(); ++j)
        {
            x += c.get(j)->getX() * bernstein(i, this->m_curves.size()-1, u) * bernstein(j, c.getSize()-1, v);
            y += c.get(j)->getY() * bernstein(i, this->m_curves.size()-1, u) * bernstein(j, c.getSize()-1, v);
            z += c.get(j)->getZ() * bernstein(i, this->m_curves.size()-1, u) * bernstein(j, c.getSize()-1, v);
        }
    }
    p.setX(x);
    p.setY(y);
    p.setZ(z);

    return p;
}

/**
 * \brief   Discrétise le carreau de Bezier
 * \details Applique la discrétisation du carreau de Bezier.
 *          La fonction remplie la liste de points discrétisés.
 */
void Discretisation::discr()
{
    QVector<Point> points;

    float step = 1.0/this->nb_elements;

    //PB: pour u=1 et v=1 -> arrêt de la boucle
    for (float u = 0.0; u <= 1.0+(step/2); u += step)
    {
        for (float v = 0.0; v <= 1.0+(step/2); v += step)
            points.push_back(discr_step(u, v));
    }

    this->pts = points;
}

/**
 * \brief   Renvoie le nombre de discrétisation par courbe
 * \return  Le nombre d'élements
 */
int Discretisation::getN()
{
    return this->nb_elements;
}

/**
 * \brief   Renvoie le nombre de point discrétisés
 * \return  Le nombre de points
 */
int Discretisation::getSize()
{
    return this->pts.size();
}

/**
 * \brief   Renvoie les points discrétisés
 * \return  Le vecteur de \e Point discrétisés
 */
QVector<Point> Discretisation::getPoints()
{
    return this->pts;
}

/**
 * \brief   Renvoie la liste des courbes
 * \return  La liste de \e CourbeParametrique
 */
QList<CourbeParametrique> Discretisation::getCarreau()
{
    return this->m_curves;
}
