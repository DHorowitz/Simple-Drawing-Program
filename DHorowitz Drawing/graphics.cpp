#include "graphics.h"
#include "drawing.h"

#include <GL/glut.h>

#include <cfloat>
#include <cmath>

using namespace std;

const colorType HIGHLIGHT_COLOR = WHITE;

const double TRANSFORM_GISMO_SCALE = 25.0;
	
const double PI = 3.14159;	
double delta = 2 * PI / 360;

bool highlight = false;

bool getHighlight() {return highlight;}

void setHighlight(bool h)
{
  if (h) setColor(HIGHLIGHT_COLOR);
  highlight = h;
}


colorType intToColor(int item)
{
  switch ( item ) {
    case 0 : return BLACK   ;
    case 1 : return BLUE    ;
    case 2 : return GREEN   ;
    case 3 : return CYAN    ;
    case 4 : return RED     ;
    case 5 : return MAGENTA ;
    case 6 : return YELLOW  ;
    case 7 : return WHITE   ;
    case 8 : return GRAY    ;
   default : return BLACK   ;
  }
}


void setColor( colorType color )
{
  if (!highlight)
    switch (color)
    {
      case BLACK   : glColor3f( 0.0, 0.0, 0.0 ); break;
      case BLUE    : glColor3f( 0.0, 0.0, 1.0 ); break;
      case GREEN   : glColor3f( 0.0, 1.0, 0.0 ); break;
      case CYAN    : glColor3f( 0.0, 1.0, 1.0 ); break;
      case RED     : glColor3f( 1.0, 0.0, 0.0 ); break;
      case MAGENTA : glColor3f( 1.0, 0.0, 1.0 ); break;
      case YELLOW  : glColor3f( 1.0, 1.0, 0.0 ); break;
      case WHITE   : glColor3f( 1.0, 1.0, 1.0 ); break;
      case GRAY    : glColor3f( 0.5, 0.5, 0.5 ); break;
    }
}


TransformStack transformStack;

void gPush(Matrix* transform)
{
	if (transformStack.empty()) {
		transformStack.push(transform);
	}
	transform = transformStack.top()->multiply(transform);
	transformStack.push(transform);
}

void gPop() {transformStack.pop();}

void drawLine(double x0, double y0, double x1, double y1)
{
	Vector* p0 = new Vector(x0, y0);
	Vector* p1 = new Vector(x1, y1);
	drawLine(p0, p1);
}

void drawLine(Vector* p0, Vector* p1)
{
	p0 = transformStack.top()->multiply(p0);
	p1 = transformStack.top()->multiply(p1);

	glBegin(GL_LINES);
	glVertex2d((*p0)[0], (*p0)[1]);
	glVertex2d((*p1)[0], (*p1)[1]);
	glEnd();

	glFlush();
}

void drawRectangle(double x0, double y0, double x1, double y1)
{
	Vector* p0 = new Vector(x0, y0);
	Vector* p1 = new Vector(x1, y0);
	Vector* p2 = new Vector(x1, y1);
	Vector* p3 = new Vector(x0, y1);

	list<Vector*> rect;
	rect.push_back(p0);
	rect.push_back(p1);
	rect.push_back(p2);
	rect.push_back(p3);
	drawPolygon(rect);
}

void drawCircle(double x0, double y0, double x1, double y1)
{
	double radius = sqrt(((x1-x0) * (x1-x0)) + ((y1-y0) * (y1-y0)));
	drawCircle((x1 + x0) / 2, (y1 + y0) / 2, radius);
}

void drawCircle(double cX, double cY, double radius)
{
	     for (double theta = 0; theta < 2*PI; theta += delta) {

			 drawLine(cX + radius * cos(theta), 
				      cY + radius * sin(theta),
				      cX + radius * cos(theta + delta), 
					  cY + radius * sin(theta + delta));
		 }
}

void drawPolygon(const list<Vector*>& vertices, bool close)
{
	list<Vector*> :: const_iterator current = vertices.begin();
	list<Vector*> :: const_iterator next = vertices.begin();
	next++;

	while (next != vertices.end()) {
	drawLine(*current, *next);
	current++;
	next++;
	}

	if (close) {
		drawLine(*current, *vertices.begin());
	}
}

void drawTransformGismo()
{
  double halfSize = TRANSFORM_GISMO_SCALE/2.0;
  drawRectangle(-halfSize,-halfSize,halfSize,halfSize);
}
