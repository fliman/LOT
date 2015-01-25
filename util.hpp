#ifndef UTIL_HPP
#define UTIL_HPP
#include <functional>

namespace LOT
{
    template <typename F>
    inline void FUN_1LOOP(size_t iend, F fun){
        for(size_t i = 0; i < (iend); i++)  fun(i); }

    
    template <typename F>
    inline void FUN_2LOOP1(size_t iend, size_t jend, F fun){
        for(size_t i = 0; i < iend; i++) 
            for(size_t j = 0; j < jend; j++ )
                fun(i*jend+j);
    }

    template <typename F>
    inline void FUN_2LOOP2(size_t iend, size_t jend, F fun){
        for(size_t i = 0; i < iend; i++) 
            for(size_t j = 0; j < jend; j++ )
                fun(i,j);
    }

     template <typename F>
     inline void FUN_3LOOP1(size_t iend, size_t jend, size_t kend, F fun){
        for(size_t i = 0; i < iend; i++) 
            for(size_t j = 0; j < jend; j++ )
                for(size_t k = 0; k < kend; k++ )
                    fun(i*jend*kend + j * kend + k);
    }

     template <typename F>
     inline void FUN_3LOOP3(size_t iend, size_t jend, size_t kend, F fun){
        for(size_t i = 0; i < iend; i++) 
            for(size_t j = 0; j < jend; j++ )
                for(size_t k = 0; k < kend; k++ )
                    fun(i,j,k);
    }

//recursive template function return type of the pointer
//    template<typename T>
//    struct throw_stars<T*>
//   {
       // using type = typename throw_stars<T>::type;
//    };

//    template<typename T>
//    struct throw_stars<T*>{
//      using type = typename throw_stars<T>::type;
//    };

    template<size_t n>
    struct pown{
    public:
        static double val(double x) {return x*pown<n-1>::val(x);}
    };

    template<>
    struct pown<0>{
        static double val(double x ) {return 1.0;}
    }; 

    template<typename T>
    T Max(T a, T b) {
  // In real-world code, we wouldn't compare floating point values like
  // this. It would make sense to specialize this function for floating
  // point types to use approximate comparison.
        return a > b ? a : b;
    }

    template<typename T, typename... Args>
    T Max(T a, T b, Args... args) {
        return a > Max(b, args..) ? a  : Max(b, args..);
    }

    
};

#endif
