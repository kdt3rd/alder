// Copyright (c) 2023 Kimball Thurston
// SPDX-License-Identifier: MIT

#include <alder/machine.h>
#include <iostream>

////////////////////////////////////////

int main( int argc, char *argv[] )
{
    using namespace alder;
    machine tm;

    tm.define_gpio( 0, "FOO", gpio_pin::state::LOW );

    std::cout << tm << std::endl;

    return 0;
}
