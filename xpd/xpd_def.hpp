/*
// Copyright (c) 2015-2016 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef XPD_DEF_HPP
#define XPD_DEF_HPP

#if (__cplusplus <= 199711L)
#define _XPD_CPP11_NOSUPPORT_
#endif

#ifdef _XPD_CPP11_NOSUPPORT_
#define xpd_noexcept
#define xpd_nullptr NULL
#define xpd_constexpr
#define xpd_override
#define xpd_final
#define xpd_delete_f
#else
#define xpd_noexcept noexcept
#define xpd_nullptr nullptr
#define xpd_constexpr constexpr
#define xpd_override override
#define xpd_final final
#define xpd_delete_f = delete
#endif

namespace xpd
{
#ifdef _ZPD_DOUBLE_PRECISION_
    //! @brief The type used for samples during digital signal processing.
    typedef double sample;
#else
    //! @brief The type used for samples during digital signal processing.
    typedef float sample;
#endif
}


#endif // XPD_DEF_HPP
