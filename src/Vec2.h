#pragma once

class Vec2
{
public:
    float x = 0;
    float y = 0;

    Vec2();
    Vec2(float xin, float yin);

    bool operator == (const Vec2 & rhs) const;
    bool operator != (const Vec2 & rhs) const;

    Vec2 operator + (const Vec2 & rhs) const;
    Vec2 operator - (const Vec2 & rhs) const;
    Vec2 operator * (const float val) const;
    Vec2 operator / (const float val) const;

    void operator += (const Vec2 & rhs);
    void operator -= (const Vec2 & rhs);
    void operator *= (const float val);
    void operator /= (const float val);

    Vec2 rotate(const float angle) const;
    Vec2 norm() const;
    void norm();
    void scale(const float minLen, const float maxLen);
    float dist(const Vec2 & rhs) const;
    float len() const;
    void abs();

    void print() const;
};
