// Copyright (c) 2023 Kimball Thurston
// SPDX-License-Identifier: MIT

#include <alder/machine.h>
#include <sstream>

////////////////////////////////////////

namespace alder
{

////////////////////////////////////////

gpio_pin::gpio_pin( size_t idx, const char *nm, state initstate )
    : _index( idx ), _state( initstate )
{
    if ( nm )
    {
        _name = std::string( nm );
    }
    else
    {
        std::stringstream sbuf;
        sbuf << "GPIO_" << idx;
        _name = sbuf.str();
    }
}

////////////////////////////////////////

void gpio_pin::raise( void )
{
    if ( _state != state::NOT_DEFINED )
        _state = state::HIGH;
}

////////////////////////////////////////

void gpio_pin::lower( void )
{
    if ( _state != state::NOT_DEFINED )
        _state = state::LOW;
}

////////////////////////////////////////////////////////////////////////////////

void machine::define_gpio( size_t idx, const char *nm, gpio_pin::state initstate )
{
    if ( idx == size_t(-1) )
        idx = _gpio.size();

    if ( idx >= _gpio.size() )
    {
        _gpio.resize( idx + 1 );
    }

    _gpio[idx] = gpio_pin{ idx, nm, initstate };
}

////////////////////////////////////////

gpio_pin &
machine::pin( size_t idx )
{
    if ( idx >= _gpio.size() )
        define_gpio( idx, NULL, gpio_pin::state::NOT_DEFINED );
    return _gpio[idx];
}

////////////////////////////////////////

std::ostream &operator<<( std::ostream &os, const gpio_pin &p )
{
    os << p.name() << " (idx " << p.index() << "): state ";
    switch ( p.current() )
    {
        case gpio_pin::state::LOW: os << "LOW"; break;
        case gpio_pin::state::HIGH: os << "HIGH"; break;
        case gpio_pin::state::UNCONNECTED: os << "FLOATING"; break;
        case gpio_pin::state::NOT_DEFINED: os << "<UNDEFINED>"; break;
    }
    return os;
}

////////////////////////////////////////

std::ostream &operator<<( std::ostream &os, const machine &m )
{
    os << m.pins().size() << " pins:\n";
    for ( auto &p: m.pins() )
        os << p << std::endl;
    return os;
}

} // alder



