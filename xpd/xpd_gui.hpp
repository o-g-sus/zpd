/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef XPD_GUI_HPP
#define XPD_GUI_HPP

#include "xpd_object.hpp"

namespace xpd
{
    //! @brief The hui.
    //! @details In fact this just a description of an gui.
    class gui : public object
    {
    public:
        
        inline xpd_constexpr gui() xpd_noexcept : object() {}
        
        inline xpd_constexpr gui(gui const& other) xpd_noexcept : object(other) {}
        
        gui(object const& other);
        
        inline gui& operator=(gui const& other) xpd_noexcept {object::operator=(other); return *this;}
        
        gui& operator=(object const& other);
        
        symbol receive_symbol() const xpd_noexcept;
        
        symbol send_symbol() const xpd_noexcept;
        
        //! @brief The destructor.
        inline virtual ~gui() {};
    };
}

#endif // XPD_GUI_HPP
