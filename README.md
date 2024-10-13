# Micropython + lvgl GALDEANO VERSION

We have linked our custom micropython code with the code to develop the GALDEANO handheld computer. It is a submodule in the GALDEANO board code.
You have to run this commands in a linux shell to get the code.

```
git clone https://github.com/otosan-maker/lv_micropython_ebro.git
cd lv_micropython_ebro
git submodule update --init --recursive ports/esp32/boards/GALDEANO/galdeano-lv
```
## WHAT IS THIS

I have created a special handheld calculator, it runs in a version os lv_micropython that I am not able to manage with the original repository. So I have created a new one. But it is a derivate work from there.
My hardware only runs on esp32, so it is the unique port than I have customiced.
I have include a version of Eigenmath as a custom module, so we can use symbolic math inside the calculator.
In the esp32 port it is also the code (LV micropython code) to run this calculator.

## Build Instructions

### ESP32 port

For now we use ESP-IDF 4.02, to install it we need to follow this steps:
```
(in a Ubunto like distro)
 apt-get install git wget flex bison gperf python3 python3-pip python3-venv cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0

 git clone -b v4.0.2 --recursive https://github.com/espressif/esp-idf.git
 cd esp-idf
 ./install.sh esp32  
 . ./export.sh
```
Back to the Micropython firmware.
The first step is compile the cross compiler. Execute this command from the lv_micropython_ebro directory.
```
make -C mpy-cross
```
Then we cam create the firmware for our hardware
```
cd ports/esp32
make  BOARD=GENERIC_SPIRAM deploy
make  BOARD=GALDEANO       deploy
make  BOARD=M5CORE2        deploy
```
- GENERIC_SPIRAM : It is for development, it creates the firmware, but the filesystem is empty. We have to populate the filesystem with the GALDEANO_LV.
- GALDEANO: It is for production, it compiles the firmware including the stable code from GALDEANO_LV, so the code for the calculator is in the frozen area.
- M5CORE2: The M5 Core2 with a keyboard faces can run the calculator code, this is the firmware.

The main objetives in make are
- make erase:  delete the flash, it we alter the partitions, or if it is the first instalation, we have to run it first
- make clean: delete the compiled code
- make deploy: it compiles and deploys the firmware to the esp32, PORT is set to the serial port (PORT=/dev/ttyUSB0, PORT=/dev/ttyACM0)

## More information

More info about LVGL:
- Website https://lvgl.io
- GitHub: https://github.com/lvgl/lvgl
- Documentation: https://docs.lvgl.io/master/get-started/micropython.html
- Examples: https://docs.lvgl.io/master/examples.html
- More examples: https://github.com/lvgl/lv_binding_micropython/tree/master/examples

More info about lvgl Micropython bindings:
- https://github.com/lvgl/lv_binding_micropython/blob/master/README.md

Discussions about the Microptyhon binding: https://github.com/lvgl/lvgl/issues/557

More info about the unix port: https://github.com/micropython/micropython/wiki/Getting-Started#debian-ubuntu-mint-and-variants

The MicroPython project
=======================
<p align="center">
  <img src="https://raw.githubusercontent.com/micropython/micropython/master/logo/upython-with-micro.jpg" alt="MicroPython Logo"/>
</p>

This is the MicroPython project, which aims to put an implementation
of Python 3.x on microcontrollers and small embedded systems.
You can find the official website at [micropython.org](http://www.micropython.org).

WARNING: this project is in beta stage and is subject to changes of the
code-base, including project-wide name changes and API changes.

MicroPython implements the entire Python 3.4 syntax (including exceptions,
`with`, `yield from`, etc., and additionally `async`/`await` keywords from
Python 3.5). The following core datatypes are provided: `str` (including
basic Unicode support), `bytes`, `bytearray`, `tuple`, `list`, `dict`, `set`,
`frozenset`, `array.array`, `collections.namedtuple`, classes and instances.
Builtin modules include `sys`, `time`, and `struct`, etc. Select ports have
support for `_thread` module (multithreading). Note that only a subset of
Python 3 functionality is implemented for the data types and modules.

MicroPython can execute scripts in textual source form or from precompiled
bytecode, in both cases either from an on-device filesystem or "frozen" into
the MicroPython executable.

See the repository http://github.com/micropython/pyboard for the MicroPython
board (PyBoard), the officially supported reference electronic circuit board.

Major components in this repository:
- py/ -- the core Python implementation, including compiler, runtime, and
  core library.
- mpy-cross/ -- the MicroPython cross-compiler which is used to turn scripts
  into precompiled bytecode.
- ports/unix/ -- a version of MicroPython that runs on Unix.
- ports/stm32/ -- a version of MicroPython that runs on the PyBoard and similar
  STM32 boards (using ST's Cube HAL drivers).
- ports/minimal/ -- a minimal MicroPython port. Start with this if you want
  to port MicroPython to another microcontroller.
- tests/ -- test framework and test scripts.
- docs/ -- user documentation in Sphinx reStructuredText format. Rendered
  HTML documentation is available at http://docs.micropython.org.

Additional components:
- ports/bare-arm/ -- a bare minimum version of MicroPython for ARM MCUs. Used
  mostly to control code size.
- ports/teensy/ -- a version of MicroPython that runs on the Teensy 3.1
  (preliminary but functional).
- ports/pic16bit/ -- a version of MicroPython for 16-bit PIC microcontrollers.
- ports/cc3200/ -- a version of MicroPython that runs on the CC3200 from TI.
- ports/esp8266/ -- a version of MicroPython that runs on Espressif's ESP8266 SoC.
- ports/esp32/ -- a version of MicroPython that runs on Espressif's ESP32 SoC.
- ports/nrf/ -- a version of MicroPython that runs on Nordic's nRF51 and nRF52 MCUs.
- extmod/ -- additional (non-core) modules implemented in C.
- tools/ -- various tools, including the pyboard.py module.
- examples/ -- a few example Python scripts.

The subdirectories above may include READMEs with additional info.

"make" is used to build the components, or "gmake" on BSD-based systems.
You will also need bash, gcc, and Python 3.3+ available as the command `python3`
(if your system only has Python 2.7 then invoke make with the additional option
`PYTHON=python2`).

The MicroPython cross-compiler, mpy-cross
-----------------------------------------

Most ports require the MicroPython cross-compiler to be built first.  This
program, called mpy-cross, is used to pre-compile Python scripts to .mpy
files which can then be included (frozen) into the firmware/executable for
a port.  To build mpy-cross use:

    $ cd mpy-cross
    $ make

The Unix version
----------------

The "unix" port requires a standard Unix environment with gcc and GNU make.
x86 and x64 architectures are supported (i.e. x86 32- and 64-bit), as well
as ARM and MIPS. Making full-featured port to another architecture requires
writing some assembly code for the exception handling and garbage collection.
Alternatively, fallback implementation based on setjmp/longjmp can be used.

To build (see section below for required dependencies):

    $ cd ports/unix
    $ make submodules
    $ make

Then to give it a try:

    $ ./micropython
    >>> list(5 * x + y for x in range(10) for y in [4, 2, 1])

Use `CTRL-D` (i.e. EOF) to exit the shell.
Learn about command-line options (in particular, how to increase heap size
which may be needed for larger applications):

    $ ./micropython -h

Run complete testsuite:

    $ make test

Unix version comes with a builtin package manager called upip, e.g.:

    $ ./micropython -m upip install micropython-pystone
    $ ./micropython -m pystone

Browse available modules on
[PyPI](https://pypi.python.org/pypi?%3Aaction=search&term=micropython).
Standard library modules come from
[micropython-lib](https://github.com/micropython/micropython-lib) project.

External dependencies
---------------------

Building MicroPython ports may require some dependencies installed.

For Unix port, `libffi` library and `pkg-config` tool are required. On
Debian/Ubuntu/Mint derivative Linux distros, install `build-essential`
(includes toolchain and make), `libffi-dev`, and `pkg-config` packages.

Other dependencies can be built together with MicroPython. This may
be required to enable extra features or capabilities, and in recent
versions of MicroPython, these may be enabled by default. To build
these additional dependencies, in the port directory you're
interested in (e.g. `ports/unix/`) first execute:

    $ make submodules

This will fetch all the relevant git submodules (sub repositories) that
the port needs.  Use the same command to get the latest versions of
submodules as they are updated from time to time. After that execute:

    $ make deplibs

This will build all available dependencies (regardless whether they
are used or not). If you intend to build MicroPython with additional
options (like cross-compiling), the same set of options should be passed
to `make deplibs`. To actually enable/disable use of dependencies, edit
`ports/unix/mpconfigport.mk` file, which has inline descriptions of the options.
For example, to build SSL module (required for `upip` tool described above,
and so enabled by default), `MICROPY_PY_USSL` should be set to 1.

For some ports, building required dependences is transparent, and happens
automatically.  But they still need to be fetched with the `make submodules`
command.

The STM32 version
-----------------

The "stm32" port requires an ARM compiler, arm-none-eabi-gcc, and associated
bin-utils.  For those using Arch Linux, you need arm-none-eabi-binutils,
arm-none-eabi-gcc and arm-none-eabi-newlib packages.  Otherwise, try here:
https://launchpad.net/gcc-arm-embedded

To build:

    $ cd ports/stm32
    $ make submodules
    $ make

You then need to get your board into DFU mode.  On the pyboard, connect the
3V3 pin to the P1/DFU pin with a wire (on PYBv1.0 they are next to each other
on the bottom left of the board, second row from the bottom).

Then to flash the code via USB DFU to your device:

    $ make deploy

This will use the included `tools/pydfu.py` script.  If flashing the firmware
does not work it may be because you don't have the correct permissions, and
need to use `sudo make deploy`.
See the README.md file in the ports/stm32/ directory for further details.

Contributing
------------

MicroPython is an open-source project and welcomes contributions. To be
productive, please be sure to follow the
[Contributors' Guidelines](https://github.com/micropython/micropython/wiki/ContributorGuidelines)
and the [Code Conventions](https://github.com/micropython/micropython/blob/master/CODECONVENTIONS.md).
Note that MicroPython is licenced under the MIT license, and all contributions
should follow this license.
