// Author: yuxuan42
// Partners: sj57, zhiboz3
#ifndef SHAPE_H_
#define SHAPE_H_

#include<iostream>
#include<cmath>
#include<string>
#include<algorithm>
#include<list>
#include<fstream>
#include <iomanip>    

using namespace std;

//Base class
//Please implement Shape's member functions
//constructor, getName()
class Shape{
public:
	//Base class' constructor should be called in derived classes'
	//constructor to initizlize Shape's private variable 
  	Shape(string name) : name_(name) {

	}
	
  	string getName() {
		return name_;
	}
	
  	virtual double getArea() const = 0;
  	virtual double getVolume() const = 0;
	
private:
  string name_;


};

//Rectangle
//Please implement the member functions of Rectangle:
//constructor, getArea(), getVolume(), operator+, operator-

template <class T>
class Rectangle : public Shape{
public:
  	Rectangle<T>(T width = 0, T length = 0):Shape("Rectangle"), width_(width), length_(length) {

	}
	
  	double getArea() const {
		return (double) width_ * length_;
	}
	
  	double getVolume() const {
		return (double) 0; // no volume
	}
	
	Rectangle<T> operator + (const Rectangle<T>& rec) {
		return Rectangle<T>(rec.getWidth() + width_, rec.getLength() + length_);
	}
	
	Rectangle<T> operator - (const Rectangle<T>& rec) {
		return Rectangle<T>(width_ < rec.getWidth()? 0 : width_ - rec.getWidth(), length_ < rec.getLength()? 0 : length_ - rec.getLength());
	} 
	
	T getWidth() const { 
		return width_;
	}
	
	T getLength() const { 
		return length_;
	}
private:
	T width_;
    T length_;

};

//Circle
//Please implement the member functions of Circle:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

class Circle : public Shape{
public:
  	Circle(double radius = 0):Shape("Circle"), radius_(radius) {

	}
	
  	double getArea() const{
		return (double) radius_ * radius_ * M_PI;
	}
	
 	double getVolume() const{
		return (double) 0; // no volume
	}
	
  	Circle operator + (const Circle& cir){
		return Circle(radius_ + cir.getRadius());
	}
	
	Circle operator - (const Circle& cir){
		return Circle(radius_ < cir.getRadius()? 0 : radius_ - cir.getRadius()); // max(0, R1 - R2)
	} 

	double getRadius() const { 
		return (double) radius_;
	}
	
private:
	double radius_;

};



//Sphere
//Please implement the member functions of Sphere:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

class Sphere : public Shape{
public:
  	Sphere(double radius = 0):Shape("Sphere"), radius_(radius) {

	}

  	double getVolume() const {
		return (double) radius_ * radius_ * radius_ * M_PI * 4.0 / 3.0; // 4/3*pi*r^3
	}	
	
  	double getArea() const {
		return (double) 4 * M_PI * radius_ * radius_; // 4*pi*r^2
	}

	Sphere operator + (const Sphere& sph) {
		return Sphere(radius_ + sph.getRadius());
	}

	Sphere operator - (const Sphere& sph) {
		return Sphere(radius_ < sph.getRadius()? 0 : radius_ - sph.getRadius());// max(0, R1 - R2)
	} 
	
	double getRadius() const {
		return (double) radius_;
	}

private:
	double radius_;

};

//Rectprism
//Please implement the member functions of RectPrism:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

class RectPrism : public Shape{
public:
  	RectPrism(double width = 0, double length = 0, double height = 0):Shape("RectPrism"), width_(width), length_(length), height_(height) {

	}
	
  	double getVolume() const {
		return (double) width_ * length_ * height_;
	}
  	
	double getArea() const {
		return (double) 2 * (width_ * length_ + length_ * height_ + height_ * width_);
	}
	
	RectPrism operator + (const RectPrism& rectp){
		return RectPrism(width_ + rectp.getWidth(), length_ + rectp.getLength(), height_ + rectp.getHeight());
	}
	
	RectPrism operator - (const RectPrism& rectp){
		return RectPrism(width_ < rectp.getWidth()? 0 : width_ - rectp.getWidth(), 
						length_ < rectp.getLength()? 0 : length_ - rectp.getLength(), 
						height_ < rectp.getHeight()? 0 : height_ - rectp.getHeight()); // max(0, R1 - R2)
	}	
	
	double getWidth() const { 
		return (double) width_;
	}
	
	double getLength() const { 
		return (double) length_;
	}
	
	double getHeight() const { 
		return (double) height_;
	}
  
private:
  	double length_;
  	double width_;
  	double height_;

};

// Read shapes from test.txt and initialize the objects
// Return a vector of pointers that points to the objects 
static list<Shape*> CreateShapes(char* file_name) {
	//@@Insert your code here
	list<Shape*> shapes(0, NULL);
	ifstream f (file_name, std::ifstream::in);
	if (!f.is_open()) {
        cerr << "Error opening file: " << file_name << endl;
        return shapes;  // Return an empty list if error
    }
	string shapeType;
    while (f >> shapeType) {
        if (shapeType == "Rectangle") {
            double width, length;
            f >> width >> length;
            shapes.push_back(new Rectangle<double>(width, length));
        } 
		else if (shapeType == "Circle") {
            double radius;
            f >> radius;
            shapes.push_back(new Circle(radius));
        } 
		else if (shapeType == "Sphere") {
            double radius;
            f >> radius;
            shapes.push_back(new Sphere(radius));
        } 
		else if (shapeType == "RectPrism") {
            double width, length, height;
            f >> width >> length >> height;
            shapes.push_back(new RectPrism(width, length, height));
        } 
		else {
            cerr << "Unknown shapetype: " << shapeType << endl; // record unknown type
        }
    }
	f.close();
	return shapes;
}

// call getArea() of each object 
// return the max area
static double MaxArea(list<Shape*> shapes){
	double max_area = 0;
	//@@Insert your code here
    for (auto shape : shapes) { // traverse the list
        double area = shape->getArea();
        if (area > max_area) max_area = area;
    }

	return (double) max_area;
}

// call getVolume() of each object 
// return the max volume
static double MaxVolume(list<Shape*> shapes){ // travere
	double max_volume = 0;
	//@@Insert your code here
    for (auto shape : shapes) {
        double volume = shape->getVolume();
        if (volume > max_volume) max_volume = volume;
    }
	
	return (double) max_volume;
}
#endif

