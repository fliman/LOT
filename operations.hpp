#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP
#include <vector>
#include <string.h>
#include "util.hpp"
//#include "/RD_work/ywang/c++/ET_simple/cvector.hpp"

//template<typename T>
//void setZero(cvector<T>& val){
//    memset(val.v_, 0, val.len_* sizeof(T));
//}
template<typename VECT>
void setZero(VECT& vec){
    vec=0.0f;
}

template<typename T>
void setZero(std::vector<T>& val){
//vector must be contiguous otherwise fill is better
    memset(&val[0], 0, val.size() * sizeof val[0]);
    //std::fill(val.begin(), val.end(), 0);
}

template<typename T>
void negative( std::vector<T>& val){for(size_t i; i < val.size(); i++) val[i] = -val[i];}

template<typename T>
void negative(T & val){val = -val;}



template<typename T, typename S>
void update_with_ScaleDir(S alpha, const std::vector<T> & p, std::vector<T> & x)
{
    for(size_t i; i < val.size(); i++)
        x[i] += alpha * p[i];
}


template<typename T, typename S>
void update_with_ScaleDir(S alpha, const T & p, T & x)
{
    x += alpha * p;
}


template<typename T, typename S>
void assign_Dir_plus_ScaleDir(S alpha, const T & p, T & x)
{
    x = p + alpha*x;
}

template<typename T, typename S>
void  assign_ScaleDir_minus_Dir(S alpha, const T & p, T & x)
{
    x = alpha*x - p;
}

template<typename T, typename S>
void assign_Dir_plus_ScaleDir(S alpha, const std::vector<T> & p, std::vector<T> & x)
{
    FUN_1LOOP(x.size(), [&](size_t i){x[i] = p[i] + alpha*x[i];});
}

template<typename T>
float dot(T v1, T v2){
    return (v1, v2);
}

template<typename T>
float l2norm(T v){
    return dot(v,v);
}

#endif
