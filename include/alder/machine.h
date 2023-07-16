// Copyright (c) 2023 Kimball Thurston
// SPDX-License-Identifier: MIT

#pragma once

#include <map>
#include <vector>
#include <string>
#include <ostream>

////////////////////////////////////////

namespace alder
{

///
/// @brief Class gpio_pin tracks the name and state of a GPIO pin in a machine
///
class gpio_pin
{
public:
    enum class state
    {
        LOW = 0,
        HIGH,
        UNCONNECTED,
        NOT_DEFINED
    };

    gpio_pin( void ) = default;
    gpio_pin( size_t idx, const char *nm, state initstate );

    const size_t index( void ) const { return _index; }
    const char *name( void ) const { return _name.c_str(); }

    state current( void ) const { return _state; }

    void raise( void );
    void lower( void );

    static gpio_pin non_existent;

private:
    size_t       _index = size_t(-1);
    state        _state = state::NOT_DEFINED;
    std::string  _name  = {};
};

std::ostream &operator<<( std::ostream &os, const gpio_pin &p );

///
/// @brief Class uart emulates a simplistic serial device for a machine
///
class uart
{
public:
private:
};

///
/// @brief Class machine provides a definition of the machine in question
///
class machine
{
public:
    const std::vector<gpio_pin> &pins( void ) const { return _gpio; }

    void define_gpio( size_t idx, const char *nm, gpio_pin::state initstate );
    gpio_pin &pin( size_t idx );
    
private:
    std::vector<gpio_pin> _gpio;
};

std::ostream &operator<<( std::ostream &os, const machine &m );

} // namespace alder



