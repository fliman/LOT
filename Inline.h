#ifndef _LOT_INLINE_H_
#define _LOT_INLINE_H_


//=================================================================================================
//
//  INLINE SETTINGS
//
//=================================================================================================

#define _USE_STRONG_INLINE 1
#define _USE_ALWAYS_INLINE 1
//=================================================================================================
//
//  BLAZE_STRONG_INLINE KEYWORD
//
//=================================================================================================

//*************************************************************************************************
/*!\def BLAZE_STRONG_INLINE
// \brief Platform dependent setup of a strengthened inline keyword.
// \ingroup system
*/
#if _USE_STRONG_INLINE && ( defined(_MSC_VER) || defined(__INTEL_COMPILER) )
#  define LOT_STRONG_INLINE __forceinline
#else
#  define LOT_STRONG_INLINE inline
#endif
//*************************************************************************************************




//=================================================================================================
//
//  BLAZE_ALWAYS_INLINE KEYWORD
//
//=================================================================================================

//*************************************************************************************************
/*!\def BLAZE_ALWAYS_INLINE
// \brief Platform dependent setup of an enforced inline keyword.
// \ingroup system
*/
#if _USE_ALWAYS_INLINE && defined(__GNUC__)
#  define LOT_ALWAYS_INLINE __attribute__((always_inline)) inline
#else
#  define LOT_ALWAYS_INLINE BLAZE_STRONG_INLINE
#endif
//*************************************************************************************************

#endif

