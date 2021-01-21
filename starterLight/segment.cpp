#include "segment.h"

#include <cmath>


Segment::Segment()
{
    pointList = QVector<Point>(2);
}

Segment::~Segment()
{

}

Segment::Segment(const Point &p1, const Point &p2)
{
    pointList = QVector<Point>(2);

    pointList[0] = p1;
    pointList[1] = p2;
}

Segment::Segment(const Segment & s)
{
    pointList = QVector<Point>(2);

	for (unsigned i=0; i<2; ++i)
        pointList[i] = s.pointList[i];
}

Point Segment::getValue(float p)
{
    Point s;
    s.setX(this->getStart().getX()*(1-p) + this->getEnd().getX()*p);
    s.setY(this->getStart().getY()*(1-p) + this->getEnd().getY()*p);
    s.setZ(this->getStart().getZ()*(1-p) + this->getEnd().getZ()*p);
    //qDebug() << s.getX() << s.getY() << s.getZ();
    return s;
}

void Segment::setStart(const Point & p)
{
    pointList[0] = p;
}

void Segment::setEnd(const Point & p)
{
    pointList[1] = p;
}

void Segment::setN(unsigned r, const Point & p)
{
	if (r>1)
		r=1;
    pointList[r] = p;
}

Point Segment::getN(unsigned r) const
{
	if (r>1)
		r=1;

    return pointList[r];
}

Point Segment::getStart() const
{
	return getN(0);
}


Point Segment::getEnd() const
{
	return getN(1);
}

float Segment::length() const
{
    float res=0.0f;

	for (unsigned i=0; i<3; ++i)
        res += pow((pointList[1]).getN(i) - (pointList[0]).getN(i), 2.0f);

	return sqrt(res);
}

Segment& Segment::operator= (const Segment &s)
{
    for (unsigned i=0; i<2; ++i)
        pointList[i] = s.pointList[i];

    return *this;
}

std::ostream& operator<<(std::ostream& out, const Segment& s)
{
    return out << s.pointList[0] << " -- " << s.pointList[1];
}
