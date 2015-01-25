#ifndef _LOT_LINESEARCH_HPP_
#define _LOT_LINESEARCH_HPP_
#include "StopRule.hpp"
#include "Functor.hpp"

namespace LOT{

#define sqrt5 2.236067977499789696
#define golden 0.618033988749894848

//root class
struct LineSearch{
};

//various pre-defined line search functors
template<
    typename ReturnType,
    typename Funt,
    typename GradFun_t,
    typename Data_t, 
    bool ifunctor = std::is_same<Funt, Functor<Data_t, ReturnType, ReturnType>>::value,
    bool igradfunctor =  std::is_same<Funt, Functor<Data_t, ReturnType, Data_t>>::value
 >
struct GoldenSectionLineSearch:public LineSearch{
//    GoldenSectionLineSearch(){}
    static const double wolfp1 = 0.05;
    static const double wolfp2 = 0.1;
    static const double sigma = 0.9;	// back parameter
    // standard wolf condition
    LOT_ALWAYS_INLINE bool wolfcondition(
        GradFun_t gradfun,
        ReturnType f0,
        ReturnType f1,
        Data_t g0, 
        ReturnType alpha, 
        Data_t p_, 
        Data_t x_
    ){
        //Armijo condition
        bool cond1 = (f1 < (f0 + wolfp1 * alpha * dot(p_, g0)));

        Data_t temp;
        if(igradfunctor){
            temp = eval<Data_t>(gradfun, temp, alpha);
        }else{
           temp = x_ + alpha * p_;
           temp = eval<Data_t>(gradfun, temp);
        }
        
        //Curvature condition
        bool cond2 = ((wolfp2 * dot(p_, g0)) <  dot(p_, temp));
        // Sufficient descent condition
        bool cond3 =  dot(p_, temp) < 0.0;
   
        return cond1 && cond2 && cond3;
    }

    ReturnType operator()(Funt func, GradFun_t gradfun, Data_t g0_, Data_t p_, Data_t x_)
    {
         // alwasys start with 1.0 step 
       ReturnType a, b;
       a = 0.0; b = 1.0;
       ReturnType tolerance = 1e-12;
       ReturnType fa, fb, f0;

       f0 = fa = func(x_);

       // for normal function simply evaluate it
       // for functor store x_, p_ then evaluate with 1 
       fb = ifunctor ? eval<ReturnType>(func, x_, p_, 1) : eval<ReturnType>(func, x_+p_);

       //store x_, p_ 
       if(igradfunctor) eval<Data_t>(gradfun, x_, p_, 1);
 
       ReturnType i1, i2; // i2 > i1
       i1 = b - golden * (b - a);
       i2 = a + golden * (b - a);

       ReturnType f1 = eval<ReturnType>(func, x_+i1*p_, i1);
       ReturnType f2 = eval<ReturnType>(func, x_+i2*p_, i2);
       size_t outcnt = 0;

       while(1){

           size_t cnt=0;
           Stopping_Rule_GSLS<ReturnType> Stopping_Rule;
           while ( ! Stopping_Rule( a, b, tolerance) && cnt < 40) {

               if (f1 > f2) {
                   a = i1;
                   fa = f1;
                   if ( Stopping_Rule( a, b, tolerance) ) break;
                   i1 = i2;
                   f1 = f2;
                   i2 =  a + golden * (b - a);
                   f2 = eval<ReturnType>(func, x_+i2*p_, i2);
               } else {
                   b = i2;
                   fb = f2;
                   if ( Stopping_Rule( a, b, tolerance) ) break;
                   i2 = i1;
                   f2 = f1;
                   i1 = b - golden * (b - a);
                   f1 = eval<ReturnType>(func, x_+i1*p_, i1);
               }
               cnt++;
           }
           // standard wolf condition
           if(wolfcondition(gradfun, f0, f1, g0_, i1, p_, x_) || outcnt > 20) return i1;
           else{
               a = 0.0f; b = i2*sigma;
               fa = f0;
               fb = f1;
               i1 = b - golden * (b - a);
               i2 = a + golden * (b - a);
               //f1 = func(x_+i1*p_);
               f1 = eval<ReturnType>(func, x_+i1*p_, i1);
               //f2 = func(x_+i2*p_);    
               f2 = eval<ReturnType>(func, x_+i2*p_, i2);
               outcnt ++;
           }
       }
       
    }
};


};
#endif
