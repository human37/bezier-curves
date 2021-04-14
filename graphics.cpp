#include "GL/freeglut.h"
#include "bezier.h"
#include <string.h>
#include <vector>
#include <glm/glm.hpp>
#include "slider.h"
#include <sstream>

// Global Variables
double screen_x = 800;
double screen_y = 600;
double t = 0.0;
bool loopExit = false;
int timeSinceStart;
int oldTimeSinceStart = 0;
int deltaTime;

int movingBezier = -1;
int movingControlPoint = -1;
int selectedBezier = -1;

bool sliderClicked = false;

std::vector<Bezier> curves;

Slider redSlider(10, 110, screen_x - 10, 150, 1, 0, 0, 0);
Slider greenSlider(10, 60, screen_x - 10, 100, 0, 1, 0, 0);
Slider blueSlider(10, 10, screen_x - 10, 50, 0, 0, 1, 0);

void DrawRectangle(double x1, double y1, double x2, double y2)
{
	glBegin(GL_QUADS);
	glVertex2d(x1, y1);
	glVertex2d(x2, y1);
	glVertex2d(x2, y2);
	glVertex2d(x1, y2);
	glEnd();
}

void DrawText(double x, double y, const char *string)
{
	void *font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	int len, i;
	glRasterPos2d(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}

	glDisable(GL_BLEND);
}

void CreateNewBezier()
{
	if (curves.size() < 5)
	{
		Bezier b(40, 100 + 70 * (curves.size() + 1));
		curves.push_back(b);
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	redSlider.Draw();
	greenSlider.Draw();
	blueSlider.Draw();

	glColor3d(redSlider.GetValue(), greenSlider.GetValue(), blueSlider.GetValue());
	glColor3d(0.2, 0.2, 0.2);
	DrawRectangle(10, 160, screen_x - 10, screen_y - 10);

	glColor3d(0, 0, 0);

	int red = (int)(redSlider.GetValue() * 255);
	std::stringstream ss;
	ss << red;
	DrawText(screen_x - 90, 130, ss.str().c_str());

	int green = (int)(greenSlider.GetValue() * 255);
	std::stringstream ss2;
	ss2 << green;
	DrawText(screen_x - 90, 80, ss2.str().c_str());

	int blue = (int)(blueSlider.GetValue() * 255);
	std::stringstream ss3;
	ss3 << blue;
	DrawText(screen_x - 90, 30, ss3.str().c_str());

	for (size_t i = 0; i < curves.size(); i++)
	{
		curves[i].DrawControlPoints();
		curves[i].DrawCurve();
	}

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	// Reset our global variables to the new width and height.
	screen_x = w;
	screen_y = h;

	redSlider.UpdateSize(screen_x - 10);
	greenSlider.UpdateSize(screen_x - 10);
	blueSlider.UpdateSize(screen_x - 10);

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	// Set the projection mode to 2D orthographic, and set the world coordinates:
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
}

#pragma region Keyboard

// This callback function gets called by the Glut
// system whenever a key is pressed.
void asciiKeyboardDown(unsigned char c, int x, int y)
{
	switch (c)
	{
	case '+':
		CreateNewBezier();
		break;
	case 27: // escape character means to quit the program
		exit(0);
		break;
	default:
		return; // if we don't care, return without glutPostRedisplay()
	}
	//camera->ProcessKeyboard(deltaTime);
	//glutPostRedisplay();
}

void asciiKeyboardUp(unsigned char c, int x, int y)
{
	switch (c)
	{

	default:
		return; // if we don't care, return without glutPostRedisplay()
	}
	//camera->ProcessKeyboard(deltaTime);
	//glutPostRedisplay();
}

#pragma endregion Keyboard

#pragma region Mouse
// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouseButton(int mouse_button, int state, int x, int y)
{
	Point clickPoint(x, screen_y - y);
	clickPoint.m_x = x;
	clickPoint.m_y = screen_y - y;
	bool sliderChanged = false;
	sliderClicked = false;

	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// If we have a bezier selected, allow the sliders to be moved
		if (selectedBezier != -1)
		{
			if (redSlider.Click(clickPoint))
			{
				curves[selectedBezier].r = redSlider.GetValue();
				sliderChanged = true;
				sliderClicked = true;
			}
			if (greenSlider.Click(clickPoint))
			{
				curves[selectedBezier].g = greenSlider.GetValue();
				sliderChanged = true;
				sliderClicked = true;
			}
			if (blueSlider.Click(clickPoint))
			{
				curves[selectedBezier].b = blueSlider.GetValue();
				sliderChanged = true;
				sliderClicked = true;
			}
		}

		// If we didn't move a slider around, check to see if we clicked a control point
		if (!sliderChanged)
		{
			for (size_t i = 0; i < curves.size(); i++)
			{
				int sel = curves[i].IsPicked(clickPoint);
				if (sel != -1)
				{
					movingBezier = i;
					movingControlPoint = sel;
					selectedBezier = i;
					curves[i].m_selected = true;
					redSlider.SetValue(curves[i].r);
					greenSlider.SetValue(curves[i].g);
					blueSlider.SetValue(curves[i].b);
					break;
				}
				else
				{
					selectedBezier = -1;
					curves[i].m_selected = false;
					redSlider.SetValue(0);
					greenSlider.SetValue(0);
					blueSlider.SetValue(0);
				}
			}
		}
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		movingBezier = -1;
		movingControlPoint = -1;
		bool sliderClicked = false;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
	{
	}
	glutPostRedisplay();
}

void mouseMove(int x, int y)
{
	if (movingBezier != -1)
	{
		curves[movingBezier].m_points[movingControlPoint].setX(x);
		curves[movingBezier].m_points[movingControlPoint].setY(screen_y - y);
		glutPostRedisplay();
	}
	if (sliderClicked)
	{
		glm::vec2 dragPoint;
		dragPoint.x = x;
		dragPoint.y = screen_y - y;
		Point g = Point(dragPoint.x, dragPoint.y);

		if (redSlider.Click(g))
		{
			curves[selectedBezier].r = redSlider.GetValue();
		}
		if (greenSlider.Click(g))
		{
			curves[selectedBezier].g = greenSlider.GetValue();
		}
		if (blueSlider.Click(g))
		{
			curves[selectedBezier].b = blueSlider.GetValue();
		}
		glutPostRedisplay();
	}
}

void mouseWheel(int wheel, int direction, int x, int y)
{
	//wheel: the wheel number, if the mouse has only a wheel this will be zero.
	//direction : a + / -1 value indicating the wheel movement direction
	//x, y : the window mouse coordinates
}

void InitializeMyStuff()
{
	CreateNewBezier();
}

void update(int deltaTime)
{
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(10, 10);

	int fullscreen = 0;
	if (fullscreen)
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	}
	else
	{
		glutCreateWindow("bezier");
	}

	// callbacks for display
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	// callbacks for input

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(asciiKeyboardDown);
	glutKeyboardUpFunc(asciiKeyboardUp);
	//glutSpecialFunc(pressKey);
	//glutSpecialUpFunc(releaseKey);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	//glutPassiveMotionFunc(mousePassiveMove);
	glutMouseWheelFunc(mouseWheel);

	glClearColor(0, 0, 0, 1);
	InitializeMyStuff();

	while (!loopExit)
	{
		// Get Delta Time for calculations
		timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
		deltaTime = timeSinceStart - oldTimeSinceStart;
		oldTimeSinceStart = timeSinceStart;

		glutMainLoopEvent();
		update(deltaTime);
		glutPostRedisplay();
	}

	return 0;
}