#include "Vec2.h"
#include <iostream>
#include <cmath>


Vec2::Vec2() : x(0), y(0) {}
Vec2::Vec2(float xin, float yin):
  x(xin), y(yin) {}

bool Vec2::operator == (const Vec2& rhs) const
{
    return (x == rhs.x) && (y == rhs.y);
}

bool Vec2::operator != (const Vec2& rhs) const
{
    return (x != rhs.x) || (y != rhs.y);
}

Vec2 Vec2::operator + (const Vec2& rhs) const
{
    return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - (const Vec2& rhs) const
{
    return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator * (const float val) const
{
    return Vec2(x*val, y*val);
}

Vec2 Vec2::operator / (const float val) const
{
    return Vec2(x/val, y/val);
}

void Vec2::operator += (const Vec2& rhs)
{
    x += rhs.x;
    y += rhs.y;
}

void Vec2::operator -= (const Vec2& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
}

void Vec2::operator *= (const float val)
{
    x = x*val;
    y = y*val;
}

void Vec2::operator /= (const float val)
{
    x = x/val;
    y = y/val;
}

float Vec2::dist(const Vec2& rhs) const
{
    return 0;
}

Vec2 Vec2::rotate(const float angle) const
{
    float radians = angle * M_PI / 180;
    float cosAngle = std::cos(radians);
    float sinAngle = std::sin(radians);
    return Vec2(x * cosAngle - y * sinAngle, x * sinAngle + y * cosAngle);
}

Vec2 Vec2::norm() const
{
    float len = sqrt(x * x + y * y);
    return Vec2(x / len, y / len);
}

void Vec2::norm()
{
    float len = sqrt(x * x + y * y);
    x = x / len;
    y = y / len;
}

float Vec2::len() const
{
    return sqrt(x * x + y * y);
}

void Vec2::scale(const float minLen, const float maxLen)
{
    float len = this->len();
    float scale = minLen + rand() % static_cast<int>(maxLen - minLen + 1);
    x = x * scale / len;
    y = y * scale / len;
}

void Vec2::abs()
{
    x = std::abs(x);
    y = std::abs(y);
}

void Vec2::print() const
{
    std::cout << "Vec2 obj: x = " << this->x << "; y = " << this->y << std::endl;
}
