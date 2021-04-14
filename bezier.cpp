#include "bezier.h"
#include "glm/glm.hpp"

Point::Point()
    : m_x(0), m_y(0) {}

Point::Point(int x, int y)
    : m_x(x), m_y(y) {}

Bezier::Bezier(int start_x, int start_y)
{
    Point right = Point(100, 0);
    ControlPointRadius = 10.0;
    m_selected = false;
    m_points[0] = Point(start_x, start_y);
    m_points[1] = m_points[0] + right;
    m_points[2] = m_points[1] + right;
    m_points[3] = m_points[2] + right;
}

Point Bezier::Evaluate(float t)
{
    // given parameter t between 0 and 1, finds the interpolated point.
    int x = m_points[0].getX() * (1 - t) * (1 - t) * (1 - t) + 3 * m_points[1].getX() * (1 - t) * (1 - t) * t + 3 * m_points[2].getX() * (1 - t) * t * t + m_points[3].getX() * t * t * t;
    int y = m_points[0].getY() * (1 - t) * (1 - t) * (1 - t) + 3 * m_points[1].getY() * (1 - t) * (1 - t) * t + 3 * m_points[2].getY() * (1 - t) * t * t + m_points[3].getY() * t * t * t;
    Point p(x, y);
    return p;
}

void Bezier::DrawCurve()
{
    // draws the curve by approximating it at about 20 evaluation points.
    glColor3d(r, g, b);
    glBegin(GL_LINE_STRIP);
    glVertex2d(m_points[0].getX(), m_points[0].getY());
    for (int i = 1; i < 20; i++)
    {
        Point mid = Evaluate((1.0 / 20.0) * i);
        glVertex2d(mid.getX(), mid.getY());
    }
    glVertex2d(m_points[3].getX(), m_points[3].getY());
    glEnd();
}

void Bezier::DrawControlPoints()
{
    // draws the 4 control points as circles.
    if (m_selected)
    {
        glColor3d(1.0, 1.0, 1.0);
        for (int p = 0; p < 4; p++)
        {
            glBegin(GL_POLYGON);
            for (int i = 0; i < 32; i++)
            {
                double theta = (double)i / 32.0 * 2.0 * 3.1415926;
                double x = m_points[p].getX() + 10.0 * cos(theta);
                double y = m_points[p].getY() + 10.0 * sin(theta);
                glVertex2d(x, y);
            }
            glEnd();
        }
    }
    glColor3d(r, g, b);
    for (int p = 0; p < 4; p++)
    {
        glBegin(GL_POLYGON);
        for (int i = 0; i < 32; i++)
        {
            double theta = (double)i / 32.0 * 2.0 * 3.1415926;
            double x = m_points[p].getX() + 10.0 * cos(theta);
            double y = m_points[p].getY() + 10.0 * sin(theta);
            glVertex2d(x, y);
        }
        glEnd();
    }
}

int Bezier::IsPicked(Point clickPoint)
{
    for (int j = 0; j < 4; j++)
    {
        Point res = clickPoint - m_points[j];
        glm::vec2 vec = {res.m_x, res.m_y};
        float len = glm::length(vec);

        if (len < ControlPointRadius)
        {
            return j;
        }
    }
    return -1;
}