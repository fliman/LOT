#ifndef _LOT_CGMin_HPP_
#define _LOT_CGMin_HPP_
#include <vector>
#include <cmath>
//#include "util.hpp"
#include "LOT.h"
#include "LineSearch.hpp"
#include "Functor.hpp"

namespace LOT
{

#define MAXCG  100    
//========================    
// unconstraint minimization via CG
//========================

    struct Conjugator{
    };    

    //The simplest conjuate parameter generator
    //only works well for simple problem 
    // has two dot operations
    template<
        typename RetrurnType,
        typename Data_t
    >
    struct Conjugator_FR:public Conjugator{
        LOT_ALWAYS_INLINE RetrurnType operator ()(
            Data_t  _newg_, 
            Data_t  _oldg_,
            Data_t _oldp_,
            bool& zeroflag
         )
        {
              return dot(_newg_,  _newg_) / dot(_oldg_, _oldg_);
        }
    };
    
    // Dai and Yuan's formula generally has better performance
    // requires line search satisfies standard wolf-condition 
    // has two dot operations
    template<
        typename ReturnType,
        typename Data_t
    >
    struct Conjugator_DY:public Conjugator{
        LOT_ALWAYS_INLINE ReturnType operator ()(
            Data_t  _newg_, 
            Data_t  _oldg_,
            Data_t _oldp_,
            bool& zeroflag
         )
        {
            Data_t tempg = _newg_ - _oldg_;
            ReturnType val = dot(_oldp_, tempg);
            if(std::abs(val) < std::numeric_limits<ReturnType>::epsilon())
                val = ReturnType(0);
            else
                val = dot(_newg_,  _newg_) / val ;

                   
            if(val < 0.0) {std::cout<<"val:"<<val<<"\n";zeroflag = true; return ReturnType(0);}
            else return val;
        }
    };

     //Hager and Zhang's formula has the best performance
    // so far, also need wolf condition in line search 
    // has three dot product operations
    template<
        typename ReturnType,
        typename Data_t
    >
    struct Conjugator_HZ:public Conjugator{
        LOT_ALWAYS_INLINE ReturnType operator ()(
            Data_t  _newg_, 
            Data_t  _oldg_,
            Data_t _oldp_,
            bool& zeroflag
         )
        {
            Data_t tempg = _newg_ - _oldg_;
            ReturnType val = dot(_oldp_, tempg);

            if(std::abs(val) < std::numeric_limits<ReturnType>::epsilon())
                val = ReturnType(0);
            else{
                tempg = tempg - 2.0 * dot(tempg, tempg) / val * _oldp_;
                val = dot(tempg,  _newg_) / val ;
            }
                   
            if(val < 0.0) {std::cout<<"val:"<<val<<"\n";zeroflag = true; return ReturnType(0);}
            else return val;
        }
    };

//========================
//    CGMin Declaration
//========================
    template <
        typename Data_t=std::vector<double>,
        typename Scalar_t=double
        >  
    class CGMin{
    private:
        Scalar_t _alpha;                                  // step
        Scalar_t _beta;                                    // conjugate gradient parameter
        Scalar_t _beta1;                                 // conjugate gradient parameter
        Data_t _x_;                                        // Minmizer
        Data_t _g_;                                        // gradient
        Data_t _g1_;                                      //new gradient
        Data_t _p_;                                        // conjugate direction
        size_t _maxiter;                                 //maximum iteration number
        size_t _iter;                                        // iteration index
        Scalar_t _stopThreshold;                 // stop criteria
        Scalar_t _fmin;                                // mimum of the function
        bool _Initialized;
        int _RestartTime;
    public:
        CGMin():_maxiter(MAXCG),_stopThreshold(1e-6){this->_Initialized = false;}
        CGMin(Data_t x0): _x_(x0),_maxiter(MAXCG),_stopThreshold(1e-6){this->_Initialized = true;} 
        CGMin(size_t size): _maxiter(MAXCG),_stopThreshold(1e-6){_x_ = Data_t(size);}
        void initialize();
        size_t IterationNum(){return _iter;}
        void setStopThredshold(Scalar_t newthred){_stopThreshold = newthred;}
        void setMaxIterations(size_t maxiter){_maxiter = maxiter;}
        Data_t minimizer(){return _x_;}
        Scalar_t min(){return _fmin;}
        bool isStop(){return (l2norm(this->_g_) <= this->_stopThreshold) ||
                                             (this->_alpha < std::numeric_limits<Scalar_t>::epsilon()) ;}

        //  General CG minimization interface
        //  should work for both linear and nonlinear problem
        //  However, linear problem has more efficient implementation
        template<
            typename Function_t,
            typename Function_grad_t,
            typename search_Strategy_t = GoldenSectionLineSearch<Scalar_t, Function_t, Function_grad_t, Data_t>,
            typename conj_Strategy_t = Conjugator_HZ<Scalar_t, Data_t>
        >
        void minimization(
            Function_t funct, 
            Function_grad_t funct_grad,
            search_Strategy_t Linesearch = search_Strategy_t(),
            conj_Strategy_t ConjugateParam = conj_Strategy_t()
        );

        template<
            typename Function_t,
            typename Function_grad_t,
            typename Ax_op
        >
        void OneStep(
            Function_t funct, 
            Function_grad_t funct_grad,
            Ax_op Ax
        ); 

        LOT_ALWAYS_INLINE void Step0(); 

        template<typename Function_grad_t>
        LOT_ALWAYS_INLINE  void Step0_CQ(Function_grad_t  funct_grad);  
        
        template<
            typename Function_grad_t,
            typename Ax_op
        >
        LOT_ALWAYS_INLINE void OneStep_CQ(
            Function_grad_t funct_grad,
            Ax_op Ax
        ); 

        // Interface of the Convex Quadraitc problem 
        // exact line search and conjugating method  
        // exists, 
        template<
            typename Function_t,
            typename Function_grad_t,
            typename Ax_op
        >
        void minimization_CQ(
            Function_t funct, 
            Function_grad_t funct_grad,
            Ax_op Ax
        );
    };

//========================    
//implementation comes here
//========================
    template <
            typename Data_t,
            typename Scalar_t
   >  
    void CGMin<Data_t, Scalar_t>::initialize()
    {     
        setZero(this->_x_);      
        this->_iter = 0;
        this->_Initialized =true;
    }

    template <
            typename Data_t,
            typename Scalar_t
    >  
    template<typename Function_grad_t>
    LOT_ALWAYS_INLINE void CGMin<Data_t, Scalar_t>::Step0_CQ(Function_grad_t  funct_grad)
    {
        
        this->_g_ = funct_grad(this->_x_);
        this->_p_ = -this->_g_;
        this-> _beta = dot(this->_g_, this->_g_); 

    }

     template <
            typename Data_t,
            typename Scalar_t
        >  
     template<
            typename Function_grad_t,
            typename Ax_op
         >
    LOT_ALWAYS_INLINE void CGMin<Data_t, Scalar_t>::OneStep_CQ(
        Function_grad_t funct_grad,
        Ax_op Ax
        ){

            Data_t tmp = Ax(this->_p_);    
            this->_alpha = this-> _beta / dot(tmp, this->_p_); 
            update_with_ScaleDir(this->_alpha, this->_p_, this->_x_);
            this->_g_ = funct_grad(this->_x_);
            this->_beta1 = dot(this->_g_, this->_g_);
            assign_ScaleDir_minus_Dir(this->_beta1/this-> _beta, this->_g_, this->_p_);
            this-> _beta = this->_beta1; 
     }

    template<
      typename Data_t,
      typename Scalar_t
    >
    template<
        typename Function_t,
        typename Function_grad_t,
        typename search_Strategy_t,
        typename conj_Strategy_t
    >
    void CGMin<Data_t, Scalar_t>::minimization(
        Function_t funct, 
        Function_grad_t funct_grad,
        search_Strategy_t Linesearch,
        conj_Strategy_t ConjugateParam
    )
    {
	
        //intialization
        if(!this->_Initialized)
            this->initialize();

        this->_g_ = funct_grad(this->_x_);
        this->_g1_ = this->_g_;
        this->_p_ = -this->_g_;
        
        this->_iter = 0;
        this->_alpha = 1.0f;
        this->_RestartTime = 0;
        Conjugator_FR<Scalar_t, Data_t> simpleconj;
        bool zeroflag = false;
        while(this->_iter < this->_maxiter && 
                   !isStop()){

            this->_alpha = Linesearch(funct, funct_grad, this->_g_, this->_p_, this->_x_);


#ifdef LOT_CG_DEBUG         
            std::cout<<"init x:"<<_x_<<"\n"<<"init p: "<<_p_<<"\n";
#endif

            update_with_ScaleDir(this->_alpha, this->_p_, this->_x_);

#ifdef LOT_CG_DEBUG           
            std::cout<<"alpha"<<_alpha<<" update:x "<<_x_<<"\n"<<"\n";
            std::cout<<"fval: "<<funct(_x_)<<"\n";
#endif     
       
            this->_g_ = this->_g1_;
       
            this->_g1_ = funct_grad(this->_x_);

            
#ifdef LOT_CG_DEBUG
            std::cout<<"old grad: "<<_g_<<"\n"<<"newgrad: "<<_g1_<<"\n";
#endif   

            this->_beta = ConjugateParam(this->_g1_, this->_g_, this->_p_, zeroflag);


#ifdef LOT_CG_DEBUG
            std::cout<<"beta: "<<_beta<<"\n";
#endif   
            
            assign_ScaleDir_minus_Dir(this-> _beta, this->_g1_, this->_p_);                       
             this->_iter++;
        }
        
        _fmin = funct(_x_);
     }
    
    //=======
    // CG minimization for convex quadratic form (x^t A x)/2 - bx
    //=======
    template<
      typename Data_t,
      typename Scalar_t
    >
    template<
            typename Function_t,
            typename Function_grad_t,
            typename Ax_op
    >
    void CGMin<Data_t, Scalar_t>::minimization_CQ(
        Function_t funct, 
        Function_grad_t funct_grad,
        Ax_op Ax
     ) 
    {	
        //intialization
        if(!this->_Initialized)
            this->initialize();
 
        Step0_CQ(funct_grad);  
   
        while(this->_iter < this->_maxiter && 
                   !isStop()){           
            OneStep_CQ(funct_grad, Ax);
            this->_iter++;
        }
        
        _fmin = funct(_x_);
     }
};

#endif
