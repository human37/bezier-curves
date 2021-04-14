#include "GL/freeglut.h"
#include "slider.h"
#include "bezier.h"

Slider::Slider(double x1, double y1, double x2, double y2, double r, double g, double b, double v)
	: mx1(x1), my1(y1), mx2(x2), my2(y2), mr(r), mg(g), mb(b), mv(v)
{

}

void Slider::DrawRectangle(double x1, double y1, double x2, double y2)
{
	glBegin(GL_QUADS);
	glVertex2d(x1, y1);
	glVertex2d(x2, y1);
	glVertex2d(x2, y2);
	glVertex2d(x1, y2);
	glEnd();
}

void Slider::UpdateSize(double x)
{
	this->mx2 = x;
}

void Slider::Draw()
{
	glColor3d(mr, mg, mb);
	DrawRectangle(mx1, my1, mx2, my2);
	glColor3d(0, 0, 0);
	double x = mx1 + (mx2 - mx1) * mv;
	DrawRectangle(x - 2, my1 - 2, x + 2, my2 + 2);

}

bool Slider::Click(Point clickPoint)
{
	if (clickPoint.m_x >= mx1 && clickPoint.m_x <= mx2 && clickPoint.m_y >= my1 && clickPoint.m_y <= my2)
	{
		mv = (clickPoint.m_x - mx1) / (mx2 - mx1);
		return true;
	}
	return false;
}