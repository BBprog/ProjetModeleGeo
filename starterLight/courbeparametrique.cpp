#include "courbeparametrique.h"


CourbeParametrique::CourbeParametrique()
{

}

CourbeParametrique::~CourbeParametrique()
{

}

void CourbeParametrique::rotate(float angle)
{
    angle = qDegreesToRadians(angle);
    float x, y;
    for (Point *p : liste_points) {
        x = p->getX();
        y = p->getY();
        p->setX(x * cos(angle) - y * sin(angle));
        p->setY(y * cos(angle) + x * sin(angle));
    }
}

CourbeParametrique CourbeParametrique::rotateCurve(float angle)
{

}

/**
 * \brief   Renvoie la liste des points de contrôle de la courbe
 * \return  Un vecteur de \e Point;
 */
QVector<Point*> CourbeParametrique::getPoints()
{
    return this->liste_points;
}

/**
 * \brief   Renvoie la liste des segments de la courbe
 * \details Crée des segments reliant les points de contrôle de la courbe.
 *          Le parcours de la liste est ascendant.
 * \return  Une liste de \e Segment;
 */
QList<Segment> CourbeParametrique::getSegments()
{
    QList<Segment> segments;
    for (int i = 0; i < this->liste_points.size()-1; ++i)
    {
        Point *p1 = this->liste_points.at(i);
        Point *p2 = this->liste_points.at(i+1);
        segments.append(Segment(*p1, *p2));
    }
    return segments;
}

/**
 * \brief   Renvoie le nombre de points de contrôle de la courbe
 * \return  La taille de la liste
 */
int CourbeParametrique::getSize()
{
    return this->liste_points.size();
}

/**
 * \brief   Renvoie le point à l'indice \a index
 * \param   index    Indice
 * \return  Un point de la liste
 */
Point* CourbeParametrique::get(int index)
{
    return this->liste_points.at(index);
}

/**
 * \brief   Renvoie le premier point de contrôle
 * \return  Le premier point de la liste
 */
Point* CourbeParametrique::getStart()
{
    return this->liste_points.at(0);
}

/**
 * \brief   Renvoie le dernier point de contrôle
 * \return  Le dernier point de la liste
 */
Point* CourbeParametrique::getEnd()
{
    return this->liste_points.at(liste_points.size() - 1);
}

CourbeParametrique& CourbeParametrique::operator= (const CourbeParametrique &c)
{
    for (unsigned i=0; i<getSize(); ++i)
        this->liste_points.replace(i, c.liste_points.at(i));

    return *this;
}
