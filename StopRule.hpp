#ifndef _LOT_STOPRULE_HPP_
#define _LOT_STOPRULE_HPP_

//various functors for different StopRule
namespace LOT{

    struct StopRule{
    };


// default stop rule for golden section line search
    template<
        typename Type
    >
    struct Stopping_Rule_GSLS : public StopRule{

    LOT_ALWAYS_INLINE Type operator()( 
            Type x1, 
            Type x2, 
            Type tolerance
        )
        {
            Type xm = 0.5 * std::abs( x1 + x2 ); 
            if ( xm <= 1.0 ) return ( std::abs( x2 - x1 ) < tolerance ) ? 1 : 0;
            return ( std::abs( x2 - x1 ) < tolerance * xm ) ? 1 : 0;
        }
    };
}
#endif
