#include "color.hpp"
#include "object.hpp"
#include "scene.hpp"
#include <math.h>   // smallpt, a Path Tracer by Kevin Beason, 2008
#include <stdlib.h> // Make : g++ -O3 -fopenmp smallpt4k.cpp -o smallpt4k
#include <stdio.h>  //        Remove "-fopenmp" for g++ version < 4.2
// Usage: time ./smallpt4k && xv image.ppm
inline double clamp(double x){ return x<0 ? 0 : x>1 ? 1 : x; }
inline int toInt(double x){ return int(pow(clamp(x),1/2.2)*255+.5); }
inline const Object* intersect(const Ray &r, double &t){
  double d; t=INFINITY;
  const Object* o=nullptr, *_;
  for(auto&i:objects) if((d=i->intersect(r,_))<t){t=d;o=_;}
  return o;
}
static double radiance(const Ray &r, const RayColor &rc, int depth, unsigned short *Xi){
  double t;                               // distance to intersection
  const Object* id;
  if (!(id=intersect(r, t))) {return rc.a(Sky.x,Sky.y,Sky.z);}; // if miss, return sky
  const Object &obj = *id;        // the hit object
  Vec x=r.o+r.d*t, n=obj.vn(x), nl=n.dot(r.d)<0?n:n*-1;
  // double p = f.x>f.y && f.x>f.z ? f.x : f.y>f.z ? f.y : f.z; // max refl
  double p = rc.a(obj.c.x, obj.c.y, obj.c.z), q = rc.a(obj.e.x, obj.e.y, obj.e.z);
  if (++depth>5) if (erand48(Xi)<p) p = 1; else return q; //R.R.
  if (obj.refl == DIFF){                  // Ideal DIFFUSE reflection
    double r1=2*M_PI*erand48(Xi), r2=erand48(Xi), r2s=sqrt(r2);
    Vec w=nl, u=((fabs(w.x)>.1?Vec(0,1):Vec(1))%w).norm(), v=w%u;
    Vec d = (u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrt(1-r2)).norm();
    return q + p * radiance(Ray(x,d),rc,depth,Xi);
  } else if (obj.refl == SPEC)            // Ideal SPECULAR reflection
    return q + p * radiance(Ray(x,r.d-n*2*n.dot(r.d)),rc,depth,Xi);
  Ray reflRay(x, r.d-n*2*n.dot(r.d));     // Ideal dielectric REFRACTION
  bool into = n.dot(nl)>0;                // Ray from outside going in?
  double nc=1, nt=rc.n, nnt=into?nc/nt:nt/nc, ddn=r.d.dot(nl), cos2t;
  if ((cos2t=1-nnt*nnt*(1-ddn*ddn))<0)    // Total internal reflection
    return q + p * radiance(reflRay,rc,depth,Xi);
  Vec tdir = (r.d*nnt - n*((into?1:-1)*(ddn*nnt+sqrt(cos2t)))).norm();
  double a=nt-nc, b=nt+nc, R0=a*a/(b*b), c = 1-(into?-ddn:tdir.dot(n));
  double Re=R0+(1-R0)*c*c*c*c*c,Tr=1-Re,P=.25+.5*Re,RP=Re/P,TP=Tr/(1-P);
  return q + p * (depth>2 ? (erand48(Xi)<P ?   // Russian roulette
    radiance(reflRay,rc,depth,Xi)*RP:radiance(Ray(x,tdir),rc,depth,Xi)*TP) :
    radiance(reflRay,rc,depth,Xi)*Re+radiance(Ray(x,tdir),rc,depth,Xi)*Tr);
}
int main() {
  int w=1920*4, h=1080*4, samps = 100/4; // # samples
  double zoom = 1.08;
  Ray cam(Vec(50+1.5,52+150,295.6-30), Vec(0,-0.042612-0.7,-1).norm()); // cam pos, dir
  Vec cx=Vec(w*(.5135/zoom)/h), cy=(cx%cam.d).norm()*(.5135/zoom), r,
    *c=(Vec*)malloc(sizeof(Vec)*w*h);
#pragma omp parallel for schedule(dynamic, 1) private(r)       // OpenMP
  for (int y=0; y<h; y++){                       // Loop over image rows
    fprintf(stderr,"\rRendering (%d spp) %5.2f%%",samps*4,100.*y/(h-1));
    for (unsigned short x=0, Xi[3]={0,0,y*y*y}; x<w; x++) {  // Loop cols
      for (int sy=0, i=(h-y-1)*w+x; sy<2; sy++)     // 2x2 subpixel rows
        for (int sx=0; sx<2; sx++, r=Vec()){        // 2x2 subpixel cols
          for (int s=0; s<samps; s++){
            double r1=2*erand48(Xi), dx=r1<1 ? sqrt(r1)-1: 1-sqrt(2-r1);
            double r2=2*erand48(Xi), dy=r2<1 ? sqrt(r2)-1: 1-sqrt(2-r2);
            Vec d = cx*( ( (sx+.5 + dx)/2 + x)/w - .5) +
                    cy*( ( (sy+.5 + dy)/2 + y)/h - .5) + cam.d;
            RayColor rc(RayColor::sample(Xi));
            r = r + Vec(rc.r, rc.g, rc.b) * radiance(Ray(cam.o,d.norm()),rc,0,Xi)*(1./samps);
          } // Camera rays are pushed ^^^^^ forward to start in interior
          c[i] = c[i] + Vec(clamp(r.x),clamp(r.y),clamp(r.z))*.25;
        }
    }
  }
  FILE *f = fopen("image.ppm", "w");         // Write image to PPM file.
  if (!f) return 1;
  fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
  for (int i=0; i<w*h; i++)
    fprintf(f,"%d %d %d ", toInt(c[i].x), toInt(c[i].y), toInt(c[i].z));
  fclose(f);
  return 0;
}
