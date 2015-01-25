#ifndef _LOT_FUNCTOR_HPP_
#define _LOT_FUNCTOR_HPP_
namespace LOT{
    template<
        typename Data_t,
        typename Scalar_t,
        typename Return_t
    >
    struct Functor{
        Data_t _x_;
        Data_t _d_;
        Scalar_t alpha_;
        
        Return_t operator()(Data_t&& y){}
        Return_t operator()(
            const Data_t&& x, 
            const Data_t&& d, 
            const Scalar_t alpha
        ){}
        Return_t operator()(
            const Data_t&& dum,
            const Scalar_t alpha
        ){}
    };
//specialization for 3 parameters fucntor ()
    template<
	typename ReturnType,
        typename T,
        typename S
    >
    ReturnType eval(Functor<T, S, S> fun, T a, T b, S s){
        return fun(a, b ,s);
    }
//specilization for functor
    template<
	typename ReturnType,
        typename T,
        typename S
    >
    ReturnType eval(Functor<T, S, S> fun, T a, S s){
        return fun(a, s);
    }
    // general form
    template<
	typename ReturnType,
        typename Funt,
        typename T,
        typename... Args
    >
    ReturnType eval(Funt fun, T a, Args... args){
        return fun(a);
    }
};
#endif
