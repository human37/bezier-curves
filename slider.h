#pragma once
#include <glm/glm.hpp>
#include "bezier.h"

class Slider
{
public:
	Slider(double x1, double y1, double x2, double y2, double r, double g, double b, double v);
	void Draw();
	bool Click(Point clickPoint);
	double GetValue() { return mv; }
	void UpdateSize(double x);
	void SetValue(double val) { mv = val; }
private:
	double mx1, my1, mx2, my2, mr, mg, mb, mv;
	void DrawRectangle(double x1, double y1, double x2, double y2);
};