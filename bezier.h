#include <GL/freeglut.h>
#include "math.h"
#pragma once

class Point
{
public:
    Point();
    Point(int x, int y);
    int m_x;
    int m_y;
    int getX() { return m_x; }
    int getY() { return m_y; }
    void setX(int x) { m_x = x; }
    void setY(int y) { m_y = y; }
    Point operator+ (const Point & first) const
    {
        return Point(m_x + first.m_x, m_y + first.m_y);
    }
    Point operator- (const Point & first) const
    {
        return Point(m_x - first.m_x, m_y - first.m_y);
    }
};



class Bezier
{
public:
    double r, g, b;
    bool m_selected;
    Point m_points[4];
    Bezier(int start_x, int start_y);
    Point Evaluate(float t);
    void DrawCurve();
    void DrawControlPoints();
    int IsPicked(Point clickPoint);
    float ControlPointRadius;
};