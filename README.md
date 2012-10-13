libcheats
=========

libcheats is a portable C library to read, manipulate, and write cheat codes in
text format.

The format is quite simple:

    "Game title 1"
    Cheat description 1
    Cheat code 1
    Cheat code 2
    Cheat description 2
    Cheat code 1
    Cheat code 2

    "Game title 2"
    etc.

Also, C++-style comments are allowed; all text beginning with a `//` sequence to
the end of the line is ignored.

Example:

    "TimeSplitters"
    // some senseless comment
    Mastercode
    902D51F8 0C0B95F6
    Invincible
    203C8728 00000001

Although libcheats is quite generic, the current version expects one code line
to be two 32-bit words. Therefore, you can only use it for systems like the PS2.
Without this size limitation, I hope that libcheats will someday become a standard
library when it comes to cheat code parsing/processing.


Build
-----

To compile libcheats, you need CMake and a C compiler like gcc.

I recommend doing an out-of-source build:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

To install libcheats:

    # make install


Usage
-----

Simply link the library to your project and include the header files.

Take a look at the source files in the test directory for working samples.


License
-------

libcheats is licensed under the terms of the MIT License. See LICENSE file.


Contact
-------

* Web: <http://mlafeldt.github.com/libcheats>
* Mail: <mathias.lafeldt@gmail.com>
* Twitter: [@mlafeldt](https://twitter.com/mlafeldt)
