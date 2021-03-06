#pragma once

#ifndef T3INELASTICDD_DB_HH
#define T3INELASTICDD_DB_HH

#include <iostream>
//#include "T3RNG.h"
#include "T3NSGangular_RWnode.hh"

//#pragma acc routine seq
//unsigned int Rand32(unsigned int & xn);
//#pragma acc routine seq
//double RND01(unsigned int & xn);
//#pragma acc routine seq
//double rndv(unsigned int xn);

class T3Inelasticdd_DB
{
public:
  T3Inelasticdd_DB(T3double xmin=-1.0, T3double xmax=1.0):Xmin(xmin),Xmax(xmax){}
  T3Inelasticdd_DB & operator=(T3Inelasticdd_DB const & nodedb);
  const size_t _num_point = 127;
  const size_t _num_nodes = 512;
#ifdef OPENACC  
  #pragma acc routine seq
#endif
  T3double RandomizeCost_node(unsigned int & generator_seed, size_t node_num) const;
#ifdef OPENACC
  #pragma acc routine seq
#endif
  T3double RandomizeCost(unsigned int & generator_seed, T3double E) const;
  void Set_V(size_t node_num, size_t point_num, T3double val) {V[node_num][point_num]=val;}
  void Set_a(size_t node_num, size_t point_num, T3double aval) {a[node_num][point_num]=aval;}
  void Set_b(size_t node_num, size_t point_num, T3double bval) {b[node_num][point_num]=bval;}
  void Set_c(size_t node_num, size_t point_num, T3double cval) {c[node_num][point_num]=cval;}
  void Set_pr(size_t node_num, T3double prval) {pr[node_num]=prval;}
  void Set_sl(size_t node_num, T3double slval) {sl[node_num]=slval;}
  void Set_Einc(size_t node_num, T3double Eincval) {Einc[node_num]=Eincval;}
  T3double Get_V(size_t node_num, size_t point_num) const {return V[node_num][point_num];}
  T3double Get_a(size_t node_num, size_t point_num) const {return a[node_num][point_num];}
  T3double Get_b(size_t node_num, size_t point_num) const {return b[node_num][point_num];}
  T3double Get_c(size_t node_num, size_t point_num) const {return c[node_num][point_num];}
  T3double Get_pr(size_t node_num) const {return pr[node_num];}
  T3double Get_sl(size_t node_num) const {return sl[node_num];}
  T3double Get_Einc(size_t node_num) const {return Einc[node_num];}
  T3double GetXmin(size_t node_num) const {return Xmin;}
  T3double GetXmax(size_t node_num) const {return Xmax;}
private:
  T3double V[512][127];
  T3double a[512][127];
  T3double b[512][127];
  T3double c[512][127];
  T3double pr[512];
  T3double sl[512];
  T3double Einc[512];
  //it is necessary for using this class for different partial sums
  //with different xmin and xmax.
  //TPT2 had, as i understand, xmin=-1, xmax=1 ( cos(theta_cm) from -1 to 1 ).
  //We with our elastic scattering approximation partial sums have now:
  //ln(1-cos(theta_cm))_min=10^(-3) and
  //ln(1-cos(theta_cm))_max=1.
  //So, to use TPT2 quadratic interpolation, we must have a possibility
  //to set different dY, dCosT, ct and fs.
  //For this purpose, we use the following variables:
  T3double Xmin;
  T3double Xmax;
};

#endif
