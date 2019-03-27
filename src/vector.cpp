#include "vector.h"
#include <assert.h>
#include <math.h>

Vector::Vector( float x, float y, float z)
{
    this->X = x;
    this->Y = y;
    this->Z = z;
}

Vector::Vector()
{}

float Vector::dot(const Vector& v) const
{
    float skalar = this->X * v.X + this->Y * v.Y + this->Z * v.Z;
    return skalar;
}

Vector Vector::cross(const Vector& v) const
{
    float x = this->Y * v.Z - this->Z * v.Y;
    float y = this->Z * v.X - this->X * v.Z;
    float z = this->X * v.Y - this->Y * v.X;
    
	return Vector(x,y,z);
}



Vector Vector::operator+(const Vector& v) const
{
    float x = this->X + v.X;
    float y = this->Y + v.Y;
    float z = this->Z + v.Z;
	return Vector(x,y,z);
}

Vector Vector::operator-(const Vector& v) const
{
    float x = this->X - v.X;
    float y = this->Y - v.Y;
    float z = this->Z - v.Z;
    return Vector(x,y,z);
}

Vector Vector::operator*(float c) const
{
    float x = this->X * c;
    float y = this->Y * c;
    float z = this->Z * c;
    return Vector(x,y,z);
}

Vector Vector::operator-() const
{
	return Vector(-this->X, -this->Y, -this->Z);
}

Vector& Vector::operator+=(const Vector& v)
{
    this->X += v.X;
    this->Y += v.Y;
    this->Z += v.Z;
	return *this;
}



Vector& Vector::normalize()
{
    (*this) = (*this) * (1/this->length());
	return *this;
}

float Vector::length() const
{
    return sqrtf(this->lengthSquared());
}

float Vector::lengthSquared() const
{
    return this->X * this->X + this->Y * this->Y + this->Z * this->Z;
}

Vector Vector::reflection( const Vector& normal) const
{
    Vector result = *this - (normal * (dot(normal)) * 2.0);
	return result.normalize();
}

bool Vector::triangleIntersection( const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s) const
{
    Vector u = b - a;
    Vector v = c - a;
    Vector n = u.cross(v);//(b-a)x(c-a)
    n = n.normalize();

    s = (a.dot(n) - n.dot(*this)) / n.dot(d);//(a°n - n°o) / n°d
    if (s <= 0) {
        return false;
    }
    Vector p = (*this) + d*s;
    // Ebenengleichungen
    Vector triangleArea = u.cross(c - a); //(b-a)x(c-a)
    float abc = triangleArea.length() / 2;
    
    triangleArea = (p - a).cross(u); //(p-a)x(b-a)
    float abp = triangleArea.length() / 2;
    
    triangleArea = (p - a).cross(c - a);//(p-a)x(c-a)
    float acp = triangleArea.length() / 2;
    
    triangleArea = (c - b).cross(b - p);//(c-b)x(b-p)
    float bcp = triangleArea.length() / 2;
    
    if (abc - (abp + acp + bcp) < 0.00001 && abc - (abp + acp + bcp) > -0.00001) {
        return true;
    }
    else return false;
}
