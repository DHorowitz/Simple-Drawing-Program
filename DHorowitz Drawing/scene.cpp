#include <iostream>
#include <algorithm>
#include <GL/glut.h>
#include "scene.h"

int TransformNode::count=0;
map <int, TransformNode*>  TransformNode::idNodeTable;

TransformNode::TransformNode(TransformNode* p)
{
	parent = p;
	shapeNode = NULL;
	selected = false;
	matrix = new Matrix();
	identifier = count++;
	idNodeTable[identifier] = this;
}

TransformNode::TransformNode(TransformNode* p, ShapeNode* s, Matrix* t)
{
	parent = p;
	shapeNode = s;
	matrix = t;
	selected = false;
	identifier = count++;
	idNodeTable[identifier] = this;
}

// As all shapes are being created as matrices, this
// delete function is unnecessary 
TransformNode::~TransformNode(){}


void TransformNode::translate(double deltaX, double deltaY)
{
	matrix = Matrix::translation(deltaX, deltaY)->multiply(matrix);
}

void TransformNode::rotate(double theta)
{
	matrix = Matrix::rotation(theta)->multiply(matrix);
}

void TransformNode::shear(double shearXY, double shearYX)
{
	matrix = Matrix::shearing(shearXY, shearYX)->multiply(matrix);
}

void TransformNode::scale(double scaleX, double scaleY)
{
	matrix = Matrix::scaling(scaleX, scaleY)->multiply(matrix);
}

void TransformNode::draw(bool displayHelpers) const
{ 
	gPush(matrix);
	glPushName(identifier);
	bool highlight = getHighlight();

	if (selected) {
		setHighlight(selected);
	}

	if (shapeNode != NULL) {
		shapeNode->draw();
	} 

	for (list<TransformNode*> :: const_iterator iter = children.begin();
		iter != children.end(); iter++)
		(*iter)->draw(displayHelpers);

	setHighlight(highlight);
	glPopName();
	gPop();
}

TransformNode* TransformNode::getParent() const 
{ 
	return parent;
}

void TransformNode::setParent(TransformNode* p) 
{ 
	parent = p;
}

void TransformNode::changeParent(TransformNode* newParent)
{
	parent = newParent;
}

void TransformNode::groupObjects(set<TransformNode*>& groupMembers)
{
  TransformNode* temp = new TransformNode (this);
  for (set<TransformNode*> :: iterator iter = groupMembers.begin();
		iter != groupMembers.end();
		iter++) { 

			temp->children.push_back(*iter);
			// TO DO: add matrix to children

			this->children.remove(*iter);

  }
  this->children.push_back(temp);
}

Matrix* TransformNode::getTransform() const
{
	return matrix;
}

TransformNode* TransformNode::clone() const
{
	ShapeNode* temp = shapeNode;
	Matrix* matemp = matrix;
	TransformNode* const copy = new TransformNode(NULL, temp, matemp);
	return copy;
}

void TransformNode::addChild(TransformNode* child)
{
	children.push_back(child);
}

void TransformNode::removeChild(TransformNode* child)
{
	children.remove(child);
}

TransformNode* TransformNode::firstChild() const
{
   return children.front();
}

TransformNode* TransformNode::lastChild() const
{
	return children.back();
}

TransformNode* TransformNode::nextChild(TransformNode* child) const
{
     list<TransformNode*> :: const_iterator iter = children.begin();

	 for (iter; (*iter) != child; ++iter) 
		{ }

	 iter++;

	 if (iter == children.end()) {
		 iter = children.begin();
	 }

	 return (*iter);
}

TransformNode* TransformNode::previousChild(TransformNode* child) const
{
     list<TransformNode*> :: const_iterator iter = children.begin();

	 for (iter; (*iter) != child; ++iter) 
		{ }

	 iter--;

	 if (iter == children.end()) {
		 iter = children.begin();
	 }

	 return (*iter);
}


void TransformNode::select() 
{
	selected = true;
}

void TransformNode::deSelect() 
{
	selected = false;
}

TransformNode* TransformNode::nodeLookup(int identifier)
{
	return idNodeTable[identifier];
}


ShapeNode::ShapeNode(colorType c)
	: color(c)
{
}

void ShapeNode::setTransformNode(TransformNode* tn)
{
	transformNode = tn;
}

TransformNode* ShapeNode::getTransformNode()
{
	return transformNode;
}


Line::Line(double xx0, double yy0, double xx1, double yy1, colorType c)
	: ShapeNode(c), x0 (xx0), y0 (yy0), x1(xx1), y1(yy1)
{
	
}


ShapeNode* Line::clone() const
{
	return (Line*)(x0, y0, x1, y1, color);
}

void Line::draw() const
{  
	setColor(color);
	drawLine(x0, y0, x1, y1);
}



Rectangle::Rectangle(double xx0, double yy0, double xx1, double yy1, colorType c)
	: ShapeNode(c), x0(xx0), y0(yy0), x1(xx1), y1(yy1)
{
}


ShapeNode* Rectangle::clone()  const
{
   return (Rectangle*)(x0, y0, x1, y1, color);
}

void Rectangle::draw() const
{
	setColor(color);
	drawRectangle(x0, y0, x1, y1);
}

Circle::Circle(double ccX, double ccY, double r, colorType c) 
	: ShapeNode(c), cX(ccX), cY(ccY), radius(r)
{

}

ShapeNode* Circle::clone() const
{
   return (Circle*)(cX, cY, radius, color);
}

void Circle::draw() const
{
	setColor(color);
	drawCircle(cX, cY, radius);
}


Polygon::Polygon(const list<Vector*>& vs, colorType c) 
	: ShapeNode(c)
{
	for (list<Vector*> :: const_iterator iter = vs.begin();
		iter != vs.end(); iter++) 
		vertices.push_back(new Vector (**iter));
}

Polygon::~Polygon()
{
	for (list<Vector*> ::iterator iter = vertices.begin();
		iter != vertices.end(); iter++) {delete (*iter);}
}

ShapeNode* Polygon::clone() const
{
  return (Polygon*)(vertices, color);
}

void Polygon::draw() const
{
	setColor(color);
	drawPolygon(vertices);
}

