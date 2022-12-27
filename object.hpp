#pragma once
#include <cmath>
#include <vector>
#include <iostream>
static constexpr double eps = 1e-4;
struct Vec {        
  double x, y, z;                  // position, also color (r,g,b)
  Vec(double x_=0, double y_=0, double z_=0){ x=x_; y=y_; z=z_; }
  Vec operator+(const Vec &b) const { return Vec(x+b.x,y+b.y,z+b.z); }
  Vec operator-(const Vec &b) const { return Vec(x-b.x,y-b.y,z-b.z); }
  Vec operator*(double b) const { return Vec(x*b,y*b,z*b); }
  Vec mult(const Vec &b) const { return Vec(x*b.x,y*b.y,z*b.z); }
  Vec& norm(){ return *this = *this * (1/sqrt(x*x+y*y+z*z)); }
  double dot(const Vec &b) const { return x*b.x+y*b.y+z*b.z; } // cross:
  Vec operator%(const Vec&b) const {return Vec(y*b.z-z*b.y,z*b.x-x*b.z,x*b.y-y*b.x);}
};
struct Ray { Vec o, d; Ray(){}; Ray(Vec o_, Vec d_) : o(o_), d(d_) {} };
enum Refl_t { DIFF, SPEC, REFR };  // material types, used in radiance()
struct Object {
    Vec e, c;      // emission, color
    Refl_t refl;      // reflection type (DIFFuse, SPECular, REFRactive)
    Object(){}
    Object(Vec e_, Vec c_, Refl_t refl_): e(e_), c(c_), refl(refl_) {}
    virtual double intersect(const Ray &r, const Object* &o) const = 0; // returns distance, 0 if nohit
    virtual Vec vn(const Vec &t) const = 0; // returns normal vector
};
struct Sphere: public Object {
    double r2;       // radius**2
    Vec p;      // position
    Sphere() {}
    Sphere(double rad_, Vec p_, Vec e_, Vec c_, Refl_t refl_):
        r2(rad_*rad_), p(p_), Object(e_, c_, refl_) {}
    inline virtual double intersect(const Ray &r, const Object* &o) const override { 
        o = this;
        Vec op = p-r.o; // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
        double t, b=op.dot(r.d), det=b*b-op.dot(op)+r2;
        if (det<0) return INFINITY; else det=sqrt(det);
        return (t=b-det)>eps ? t : ((t=b+det)>eps ? t : INFINITY);
    }
    inline virtual Vec vn(const Vec &t) const override {
        return (t-p).norm();
    }
};
struct Triangle: public Object {
    Vec x, y, z, n;
    Vec xy, x_y, yz, y_z, zx, z_x;
    Triangle() {}
    Triangle(Vec x_, Vec y_, Vec z_, Vec e_, Vec c_, Refl_t refl_):
        x(x_), y(y_), z(z_), Object(e_, c_, refl_)
    {
        n = ((x-y)%(z-x)).norm();
        // std::cout << n.x << ' ' << n.y << ' ' << n.z << std::endl;
        xy=x%y,x_y=x-y,yz=y%z,y_z=y-z,zx=z%x,z_x=z-x;
    }
    inline virtual double intersect(const Ray &r,  const Object* &o) const override {
        o = this;
        double t = n.dot(x - r.o) / n.dot(r.d);
        if (t > eps) {
            double dxy=r.d.dot(xy+r.o%x_y), dyz=r.d.dot(yz+r.o%y_z), dzx=r.d.dot(zx+r.o%z_x);
            int det = (dxy>0)+(dyz>0)+(dzx>0);
            return (det==0||det==3)?t:INFINITY;
        }
        return INFINITY;
    }
    inline virtual Vec vn(const Vec &t) const override {
        return n;
    }
};
template<int N>
struct Polyhedron: public Object {
    // std::vector<Triangle> faces;
    Triangle faces[N];
    Sphere range;
    Polyhedron(std::initializer_list<Triangle> ff, Sphere r, double rty=0): range(r) {
        double sint=sin(rty), cost=cos(rty); int i=0;
        for (auto&f:ff)
            faces[i++]=(Triangle(
                {(f.x.x-r.p.x)*cost+(f.x.z-r.p.z)*sint+r.p.x, f.x.y, (f.x.z-r.p.z)*cost-(f.x.x-r.p.x)*sint+r.p.z},
                {(f.y.x-r.p.x)*cost+(f.y.z-r.p.z)*sint+r.p.x, f.y.y, (f.y.z-r.p.z)*cost-(f.y.x-r.p.x)*sint+r.p.z},
                {(f.z.x-r.p.x)*cost+(f.z.z-r.p.z)*sint+r.p.x, f.z.y, (f.z.z-r.p.z)*cost-(f.z.x-r.p.x)*sint+r.p.z},
                f.e, f.c, f.refl
            ));
    }
    inline virtual double intersect(const Ray &r, const Object* &o) const override {
        const Object* _; double t=INFINITY;
        if (range.intersect(r,_) < INFINITY) {
            double d;
            for(auto&f:faces) if((d=f.intersect(r,_))<t){t=d;o=_;}
        }
        return t;
    }
    inline virtual Vec vn(const Vec &t) const override {
        return {};
    }
};
template<int N>
struct Pointset: public Object {
    // std::vector<Sphere> points;
    Sphere points[N];
    Sphere range;
    Pointset(std::initializer_list<Sphere> ff, Sphere r): range(r) {
        int i=0;
        for (auto&f:ff) points[i++]=f;
    }
    inline virtual double intersect(const Ray &r, const Object* &o) const override {
        const Object* _;
        double t = INFINITY;
        if (range.intersect(r,_) < INFINITY) {
            double d;
            for(auto&f:points) if((d=f.intersect(r,_))<t){t=d;o=_;}
        }
        return t;
    }
    inline virtual Vec vn(const Vec &t) const override {
        return {};
    }
};