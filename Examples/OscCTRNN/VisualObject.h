// ****************************
// A class for visual objects
//
// Matthew Setzler 4/19/17
// ****************************

#include <cmath>

// A visual ray – line segment of form y = mx + b with start coordinates and length

typedef struct {
  double b;
  double m;
  double startX;
  double startY;
  double length;
} Ray;


// The VisualObject class declaration

class VisualObject {
  public:
    // The constructor
    VisualObject(double ix = 0.0, double iy = 275.0, double vy_ = -3.0, double size_ = 30.0) {
      cx = ix; cy = iy;
      vy = vy_;
      size = size_;
    };
    
    // The deconstructor
    ~VisualObject() {};

    // Updates ray length if an intersection occurs
    // Assumes agent is 'looking up' at object
    virtual void RayIntersection(Ray &ray) {};
    
    // Accessors
    void SetPositionX(double x) {cx = x;};
    void SetPositionY(double y) {cy = y;};
    double PositionX() {return cx;};
    double PositionY() {return cy;};

    void Step(double StepSize) {
      cy += StepSize * vy;
    };

  protected:
    double cx,cy,vy,size;
};


// Class for horizontal line segment

class Line : public VisualObject {
public:
  // The constructor
  // size --> length of segment
  Line(double ix = 0.0, double iy = 275.0, double vy_ = -3.0, double size_ = 30.0) 
    : VisualObject(ix,iy,vy_,size_) {};

  // The deconstructor
  ~Line() {};

  void RayIntersection(Ray &ray) {
    double x_intersect;
    if (ray.m == INFINITY) {
      x_intersect = ray.startX;
    } else {
      x_intersect = (cy - ray.b)/ray.m;
    }
    
    // No intersection
    if ((fabs(cx-x_intersect) > size/2.0) || (cy < 0)) return;

    // Intersection, return distance
    double new_length = sqrt(pow(x_intersect-ray.startX, 2.0) + pow(cy-ray.startY, 2.0));
    if (new_length < ray.length) ray.length = new_length;
  };
};


// Class definition for a circle

class Circle : public VisualObject {
public:
  // The constructor
  // size --> diameter
  Circle(double ix = 0.0, double iy = 275.0, double vy_ = -3.0, double size_ = 30.0) 
    : VisualObject(ix,iy,vy_,size_) {};
  
  // The deconstructor
  ~Circle() {};

  void RayIntersection(Ray &ray) {
    // Special case, vertical ray
    if (ray.m == INFINITY) {
      if (fabs(ray.startX-cx) > size/2) return;

      double x_intersect = ray.startX;
      double A = 1;
      double B = -2*cy;
      double C = pow(cy,2)-pow(size/2,2)+pow(x_intersect-cx,2);
      double disc = pow(B,2)-4*A*C;
      double y_intersect = (-B-sqrt(disc))/(2*A); // assuming cy>0 and cy>ray.startY

      double new_length = fabs(y_intersect-ray.startY);
      if (new_length < ray.length) ray.length = new_length;
      return;
    }

    double A = 1 + pow(ray.m, 2);
    double B = 2*(ray.m*(ray.b-cy)-cx);
    double C = pow(cx,2)+pow(ray.b,2)-2*ray.b*cy+pow(cy,2)-pow(size/2,2);
    double disc = pow(B,2)-4*A*C;

    if (disc < 0) return; // No intersection
    
    // Find points of intersection, return smaller distance (distances are the same for tangent lines)
    double x_intersect1 = (-B+sqrt(disc))/(2*A);
    double y_intersect1 = ray.m*x_intersect1 + ray.b;
    double distance1 = sqrt(pow(x_intersect1-ray.startX,2)+pow(y_intersect1-ray.startY,2));

    double x_intersect2 = (-B-sqrt(disc))/(2*A);
    double y_intersect2 = ray.m*x_intersect2 + ray.b;
    double distance2 = sqrt(pow(x_intersect2-ray.startX,2)+pow(y_intersect2-ray.startY,2));

    double new_length;
    if (distance1 < distance2) {
      new_length = distance1;
    } else {
      new_length = distance2;
    }
    if (new_length < ray.length) ray.length = new_length;
  };
};

