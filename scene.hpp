#pragma once
#include "object.hpp"

#define obj static const auto

obj Sky = Vec(.2,.2,.2);

// Sphere spheres[8]; //Scene: radius, position, emission, color, material
// wall
obj Left=Sphere(1e5, Vec( 1e5+1,40.8,81.6), Vec(),Vec(.75,.25,.25),DIFF);//Left
obj Right=Sphere(1e5, Vec(-1e5+99,40.8,81.6),Vec(),Vec(.25,.25,.75),DIFF);//Rght
obj Back=Sphere(1e5, Vec(50,40.8, 1e5),     Vec(),Vec(.25,.75,.25),DIFF);//Back
obj Front=Sphere(1e5, Vec(50,40.8,-1e5+170), Vec(),Vec(),           DIFF);//Frnt
obj Bottom=Sphere(1e5, Vec(50, -1e5, 81.6),    Vec(),Vec(.8,.2,.5),DIFF);//Botm
obj Top=Sphere(1e5, Vec(50,-1e5+81.6,81.6),Vec(),Vec(.75,.75,.75),DIFF);//Top

// ball
obj Mirror=Sphere(16.5,Vec(27,16.5,47),       Vec(),Vec(1,1,1)*.999, SPEC);//Mirr
obj Glass=Sphere(16.5,Vec(63,16.5,78),       Vec(),Vec(1,1,1)*.999, REFR);//Glas

// top light
obj Light=Sphere(600, Vec(50,681.6-.27,81.6),Vec(12,12,12),  Vec(), DIFF);//Lite

// tetrahedron
obj T1=Triangle(Vec(51.8,14.7,45.2),Vec(44,0,38.6),Vec(61.6,0,42.2),  Vec(),Vec(1,1,1)*.3,DIFF);
obj T2=Triangle(Vec(51.8,14.7,45.2),Vec(61.6,0,42.2),Vec(50,0,55.9),  Vec(),Vec(1,1,1)*.3, DIFF);
obj T3=Triangle(Vec(51.8,14.7,45.2),Vec(50,0,55.9),Vec(44,0,38.6),  Vec(),Vec(1,1,1)*.3, DIFF);
obj TT=Polyhedron<3>({T1,T2,T3},Sphere(15, {51.8,5,45.2},  Vec(),Vec(),DIFF));

// char
#define R 3
#define M 9
#define N 33
#define R0 Vec(),Vec(1,0,0)*.999, SPEC
#define R1 Vec(),Vec(1,0,0)*.999, DIFF
#define R2 Vec(),Vec(1,0,0)*.999, REFR
#define R3 Vec(1,0,0)*1.8,Vec(1,1,1)*.999, DIFF
#define CP(i,j) Vec(j%3,0,j/3)*M+Vec(51.8+i*N-1.5*N-M,R,45.2-2*M)
#define SP(i,j) Sphere(R, CP(i,j), R##i)
obj CA = Pointset<11>(
    {SP(0,0),SP(0,1),SP(0,2),SP(0,5),SP(0,6),SP(0,7),SP(0,8),SP(0,9),SP(0,12),SP(0,13),SP(0,14)},
    Sphere(M*2.24+R,CP(0,7),     Vec(),Vec(),DIFF));
obj CB = Pointset<12>(
    {SP(1,0),SP(1,1),SP(1,2),SP(1,3),SP(1,5),SP(1,6),SP(1,8),SP(1,9),SP(1,11),SP(1,12),SP(1,13),SP(1,14)},
    Sphere(M*2.24+R,CP(1,7),     Vec(),Vec(),DIFF));
obj CC = Pointset<11>(
    {SP(2,0),SP(2,1),SP(2,2),SP(2,5),SP(2,6),SP(2,7),SP(2,8),SP(2,9),SP(2,12),SP(2,13),SP(2,14)},
    Sphere(M*2.24+R,CP(2,7),     Vec(),Vec(),DIFF));
obj CD = Pointset<11>(
    {SP(3,0),SP(3,1),SP(3,2),SP(3,5),SP(3,6),SP(3,7),SP(3,8),SP(3,11),SP(3,12),SP(3,13),SP(3,14)},
    Sphere(M*2.24+R,CP(3,7),     Vec(),Vec(),DIFF));
#undef R
#undef M
#undef N
#undef CP
#undef SP
#undef R0
#undef R1
#undef R2
#undef R3

// ball(2)
#define H 65
#define E 0.25
obj MirrorBall = Sphere(15, Vec(15,15,30),  Vec(),Vec(1,1,1)*.999,REFR);
obj CenterLight = Sphere(7, Vec(51.8,H,45.2),   Vec(150,200,100),Vec(), DIFF);
obj SideBall1 = Sphere(5, Vec(48,H-1.5,71.5),  Vec(1,.843,0)*E,Vec(1,1,1)*.999,REFR);
obj SideBall2 = Sphere(5, Vec(75,H+3.6,57),    Vec(0,1,0)*E,Vec(1,1,1)*.999,REFR);
obj SideBall3 = Sphere(5, Vec(69,H-0.1,27),    Vec(0,1,1)*E,Vec(1,1,1)*.999,REFR);
obj SideBall4 = Sphere(5, Vec(39,H-1.8,23),    Vec(1,0,0)*E,Vec(1,1,1)*.999,REFR);
obj SideBall5 = Sphere(5, Vec(25.5,H+2.2,51),  Vec(.625,.32,.176)*E,Vec(1,1,1)*.999,REFR);
obj TopLight = Sphere(13, {120, 100, -20},   Vec(1,3,5),{},DIFF);
#undef E

// dodecahedron
#define M 0.5257311121*20
#define N 0.8506508084*20
#define C {51.8,H,45.2}
#define P1 {-M,0,N}
#define P2 {M,0,N}
#define P3 {-M,0,-N}
#define P4 {M,0,-N}
#define P5 {0,N,M}
#define P6 {0,N,-M}
#define P7 {0,-N,M}
#define P8 {0,-N,-M}
#define P9 {N,M,0}
#define P10 {N,-M,0}
#define P11 {-N,M,0}
#define P12 {-N,-M,0}
#define D(x,y,z) Triangle(Vec P##x + Vec C, Vec P##y + Vec C, Vec P##z + Vec C,   Vec(),Vec(1,1,1)*.999,REFR)
obj D1=D(5,1,2);
obj D2=D(5,2,9);
obj D3=D(5,9,6);
obj D4=D(6,9,4);
obj D5=D(6,4,3);
obj D6=D(6,3,11);
obj D7=D(5,6,11);
obj D8=D(1,5,11);
obj D9=D(2,10,9);
obj D10=D(4,9,10);
obj D11=D(12,11,3);
obj D12=D(1,11,12);
obj D13=D(1,7,2);
obj D14=D(2,7,10);
obj D15=D(10,7,8);
obj D16=D(4,10,8);
obj D17=D(3,4,8);
obj D18=D(12,3,8);
obj D19=D(7,12,8);
obj D20=D(12,7,1);
obj DD=Polyhedron<20>(
    {D1,D2,D3,D4,D5,D6,D7,D8,D9,D10,D11,D12,D13,D14,D15,D16,D17,D18,D19,D20},
    Sphere(20, Vec C,  Vec(),Vec(),DIFF), 0.1);
#undef M
#undef N
#undef C 
#undef P1
#undef P2
#undef P3
#undef P4
#undef P5
#undef P6
#undef P7
#undef P8
#undef P9
#undef P10
#undef P11
#undef P12
#undef D

#undef H

#undef obj

static const Object* objects[] = {
  &Bottom, &CenterLight,
//   &TopLight,
//   &MirrorBall,
//   &Bottom, &Top, &Left, &Right, &Front, &Back, &Light,
//   &TT,
  &SideBall1, &SideBall2, &SideBall3, &SideBall4, &SideBall5, 
  &DD,
  &CA,&CB,&CC,&CD,
//   &Mirror,
//   &Glass,
};