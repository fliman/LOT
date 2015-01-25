#include<functional>
#include<iostream>

template<typename T, typename ReT>
inline ReT func(T x, T y)
{
    return x+y*0.1;
}

template<typename T, typename ReT, typename... ARGS>
struct functor{
    std::function< ReT(T a, ARGS... args)> funp;
    functor(decltype(funp) fun): funp(fun){}
    inline ReT operator()(T a, ARGS... args) {return funp(a, args...);}
};

template<typename T, typename ReT, typename Functor>
struct functorfunctor{
    inline ReT operator()(Functor fun, T a, T b) {return fun(a, b);}
};

template<typename T, typename FUNC>
inline float func1(FUNC fun, T x, T y)
{
    return fun (x, y);
}

template<typename T, typename ReT> struct FunctionType
{
    typedef std::function<ReT(T, T)> Type ;
} ;

template<typename T, typename Func, typename ReT> struct FunctorType
{
    typedef std::function<ReT(Func, T, T)> Type ;
} ;

template<typename CALLFUN1, typename CALLFUN2>
float callfun(CALLFUN1 fun1, CALLFUN2 fun2)
{
    return fun1(fun2, 1, 2);
}

template<typename T>
bool pair_comparer(T a, T b) {
  // In real-world code, we wouldn't compare floating point values like
  // this. It would make sense to specialize this function for floating
  // point types to use approximate comparison.
  return a == b;
}

template<typename T, typename... Args>
bool pair_comparer(T a, T b, Args... args) {
  return a == b && pair_comparer(args...);
}

int main()
{


    std::cout<<callfun(func1<int,  FunctionType<int, float>::Type>, func<int, float>)<<"\n"; // this is not inlined, advantage of using std::function

    //functor<int, float> fun1([](int a, int b){return a+b*.02;}); // lambda function
    functor<int, float, int> fun2(func<int, float>);
    std::cout<<fun2(1, 2)<<"\n";
    //functorfunctor<int, float, functor<int, float>> funfun;
                                                                                       //func1(fun1, 1, 2);  
                                                                                       
                                                                                       //std::cout<<callfun(funfun, fun1)<<"\n"; // this can be inlined by compiler, icpc has problem with inline std::function
    //std::cout<<callfun(funfun, fun2)<<"\n"; // this can be inlined by compiler, icpc has problem with inline std::function  
    pair_comparer(1 ,2, 3, 4, 5, 6);
}
