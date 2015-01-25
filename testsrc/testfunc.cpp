
#include<iostream>
#include "Blaze.h"
//#include "util.hpp"
#include "LOT.h"
//#include "/RD_work/ywang/c++/ET_simple/cvector.hpp"


using namespace LOT;
typedef blaze::DynamicVector<float> fVec;

void test(int a) {std::cout<<"test int:"<<a<<"\n";}

template<typename T>
void test(T a) {std::cout<<"test template:"<<a<<"\n";}


float costf(const fVec& val){
    // A  | 2  1 |   b  |3   
    //      | 1  1 |       |2

    float funval = pown<2>::val(val[0])+ val[0] * val[1] +pown<2>::val(val[1])*0.5 - 2.0 * val[0] - 3.0 * val[1] ;

    return funval;
}

float nlcostf(const fVec& val){
    // A  | 2  1 |   b  |3   
    //      | 1  1 |       |2
    
    float funval = 100.0f *  pown<2>::val(val[1] - pown<2>::val(val[0])) +  pown<2>::val(val[0]-1.0f);
    return funval;
}

fVec& nlgradf(const fVec& val){
    fVec *grad = new fVec(2);

    (*grad)[0] = 400.0f * (val[0] * val[0] - val[1]) *val[0] + 2.0f * val[0] - 2.0; 
    (*grad)[1] = 200.0f *(val[1] - val[0] * val[0]);
    
    return *grad;
}


fVec & gradf(const fVec& val){
    // A  | 2  1 |   b  |3   
    //      | 1  1 |       |2

    fVec *grad = new fVec(2);

    (*grad)[0] = 2 * val[0] + 1 * val[1] - 2.0; 
    (*grad)[1] =       val[0] +       val[1] - 3.0;
    
    return *grad;
}


fVec& Ax(const fVec& val){
    // A  | 2  1 |   b  |3   
    //      | 1  1 |       |2

    fVec *grad = new fVec(2);

    (*grad)[0] = 2 * val[0] + 1 * val[1]; 
    (*grad)[1] =     val[0] +     val[1];
    
    return *grad;
}

struct testfunctor: public Functor<fVec, float, float>{
    //overload default ()
    float operator()(
        fVec && y
    ){
         float funval = 100.0f *  pown<2>::val(y[1] - pown<2>::val(y[0])) +  pown<2>::val(y[0]-1.0f);
    return funval;
    }

    //overload 3-parameter ()
    float operator()(
        const fVec&& x, 
        const fVec&& d,
        float alpha
    ){
        _x_ = x;
        _d_ = d;
        operator()(x+alpha*d);
    }

        //overload 2-parameter ()
    float operator()(
         const fVec&& dum, 
         const float alpha
    ){
        operator()(_x_+alpha*_d_);
    }

};

struct testgradfunctor: public Functor<fVec, float, fVec>{
    //overload default ()
    fVec operator()(
        fVec && y
    ){
        fVec *grad = new fVec(2);

        (*grad)[0] = 400.0f * (y[0] * y[0] - y[1]) * y[0] + 2.0f * y[0] - 2.0; 
        (*grad)[1] = 200.0f *(y[1] - y[0] * y[0]);
    
        return *grad;
    }

    //overload 3-parameter ()
    fVec operator()(
        const fVec&& x, 
        const fVec&& d,
        float alpha
    ){
        _x_ = x;
        _d_ = d;
        operator()(x+alpha*d);
    }

        //overload 2-parameter ()
    fVec operator()(
         const fVec&& dum, 
         const float alpha
    ){
        operator()(_x_+alpha*_d_);
    }

};

int main()
{
    //test FUN_LOOP s
    auto fun= [&](size_t i){std::cout<<i<<"\n";};
    //FUN_1LOOP(10, fun);
//   auto fun1 = [&](size_t i, size_t j){std::cout<<i*jend+j<<"\n";};
    //FUN_2LOOP1(5, 11, fun);
   //  auto fun2 = [&](size_t i, size_t j, size_t k){std::cout<<i*jend*kend+j*10+k<<"\n";};
    //FUN_3LOOP1(4, 3, 10, fun);
  
    fVec x(2), y(2);
    CGMin<fVec, float> cgsolver(2);


    
    cgsolver.initialize();
    cgsolver.minimization_CQ(costf, gradf, Ax);
    x = cgsolver.minimizer();	
    float fmin = cgsolver.min();


    std::cout<<x[0]<<": "<<x[1]<<"\n";
    std::cout<<"Min: "<<fmin<<"\n";

    Conjugator_FR<float, fVec> cgfr;
    //fVec& (*fungrad)(float(const fVec&), fVec, fVec)= DefaultLinesearch<float>;

    x[0] = 0.9f;x[1] = 0.8f;
    CGMin<fVec, float> cgsolvernl(x);

    //GoldenSectionLineSearch<float, fVec, std::function<int(int)>> linesearch;
    cgsolvernl.setStopThredshold(1e-8);
    testfunctor f1;
    testgradfunctor f1grad;
    cgsolvernl.minimization(f1, f1grad);
    std::cout<<"total iter: "<< cgsolvernl.IterationNum()<<"\n";
     y =  cgsolvernl.minimizer();
     std::cout<<y[0]<<": "<<y[1]<<"\n";
}
