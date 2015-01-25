#ifndef _LOT_FUNCTOR_HPP_
#define _LOT_FUNCTOR_HPP_

// Efficient Function evaluation for linesearch

namespace LOT{

// functor no really owns the data, std::move shold OK
    template<
        typename Data_t,
        typename Scalar_t,
        typename ReturnType
    >
    struct Functor{
        Data_t _x_;
        Data_t _d_;
        Scalar_t alpha_;
        //should we use pure virtual functions here?
        ReturnType operator()(Data_t y){}  // normal function evaluation
        ReturnType operator()(Data_t x, Data_t d, Scalar_t alpha){}  //evaluate x + alpha * d, and update _x_, _d_ 
        ReturnType operator()(Scalar_t alpha, int dum){}  // //evaluate x + alpha * d, but not update _x_, _d_, dum is a placeholder in case Data_t = Scalar_t

    }

}
#endif
