# Micropython + lvgl

[![Build lv_micropython unix port](https://github.com/lvgl/lv_micropython/actions/workflows/unix_port.yml/badge.svg)](https://github.com/lvgl/lv_micropython/actions/workflows/unix_port.yml)
[![Build lv_micropython stm32 port](https://github.com/lvgl/lv_micropython/actions/workflows/stm32_port.yml/badge.svg)](https://github.com/lvgl/lv_micropython/actions/workflows/stm32_port.yml)
[![esp32 port](https://github.com/lvgl/lv_micropython/actions/workflows/ports_esp32.yml/badge.svg)](https://github.com/lvgl/lv_micropython/actions/workflows/ports_esp32.yml) [![Build lv_micropython rp2 port](https://github.com/lvgl/lv_micropython/actions/workflows/rp2_port.yml/badge.svg)](https://github.com/lvgl/lv_micropython/actions/workflows/rp2_port.yml)

[![Open in Gitpod](https://gitpod.io/button/open-in-gitpod.svg)](https://gitpod.io/#https://github.com/lvgl/lv_micropython)  
With GitPod you can edit, build and run Micropython + LVGL from your web browser!

To quickly run Micropython + LVGL from your web browser you can also use the [Online Simulator](https://sim.lvgl.io/v8.1/micropython/ports/javascript/index.html).

**For information abound Micropython lvgl bindings please refer to [lv_binding_micropython/README.md](https://github.com/lvgl/lv_binding_micropython/blob/master/README.md)**

See also [Micropython + LittlevGL](https://blog.lvgl.io/2019-02-20/micropython-bindings) blog post. (LittlevGL is LVGL's previous name.)  
For questions and discussions - please use the forum: https://forum.lvgl.io/c/micropython

Original micropython README: https://github.com/micropython/micropython/blob/master/README.md

## Relationship between `lv_micropython` and `lv_binding_micropython`

Originally, `lv_micropython` was created as an example of how to use [lv_binding_micropython](https://github.com/lvgl/lv_binding_micropython) on a Micropython fork.  
As such, we try to keep changes here as minimal as possible and we try to keep it in sync with Micropython upstream releases. We also try to add changes to `lv_binding_micropython` instead of to `lv_micropython`, when possible. (for example we keep all drivers in `lv_binding_micropython`, the ESP32 CMake functionality etc.)

Eventually it turned out that many people prefer using `lv_micropython` directly and only a few use it as a reference to support LVGL on their own Micropython fork.
If you are only starting with Micropython+LVGL, it's recommended that you use `lv_micropython`, while porting a Micropython fork to LVGL is for advanced users.

## Build Instructions

### Unix (Linux) port

1. `sudo apt-get install build-essential libreadline-dev libffi-dev git pkg-config libsdl2-2.0-0 libsdl2-dev python3.8 parallel`
Python 3 is required, but you can install some other version of python3 instead of 3.8, if needed.
2. `git clone https://github.com/lvgl/lv_micropython.git`
3. `cd lv_micropython`
4. `git submodule update --init --recursive lib/lv_bindings`
5. `make -C mpy-cross`
6. `make -C ports/unix submodules`
7. `make -C ports/unix`
8. `./ports/unix/micropython`

### ESP32 port

Please set `ESPIDF` parameter for the esp-idf install dir.
It needs to match Micropython expected esp-idf, otherwise a warning will be displayed (and build will probably fail)
For more details refer to [Setting up the toolchain and ESP-IDF](https://github.com/lvgl/lv_micropython/blob/master/ports/esp32/README.md#setting-up-the-toolchain-and-esp-idf)

When using IL9341 driver, the color depth and swap mode need to be set to match ILI9341. This can be done from the command line.
Here is the command to build ESP32 + LVGL which is compatible with ILI9341 driver:

```
make -C mpy-cross
make -C ports/esp32 LV_CFLAGS="-DLV_COLOR_DEPTH=16 -DLV_COLOR_16_SWAP=1" BOARD=GENERIC_SPIRAM deploy
```

Explanation about the paramters:
- `LV_CFLAGS` are used to override color depth and swap mode, for ILI9341 compatibility.
  - `LV_COLOR_DEPTH=16` is needed if you plan to use the ILI9341 driver.
  - `LV_COLOR_16_SWAP=1` is needed if you plan to use the [Pure Micropython Display Driver](https://blog.lvgl.io/2019-08-05/micropython-pure-display-driver).
- `BOARD` - I use WROVER board with SPIRAM. You can choose other boards from `ports/esp32/boards/` directory.
- `deploy` - make command will create ESP32 port of Micropython, and will try to deploy it through USB-UART bridge.

For more details please refer to [Micropython ESP32 README](https://github.com/micropython/micropython/blob/master/ports/esp32/README.md).

### JavaScript port

Refer to the README of the `lvgl_javascript` branch: https://github.com/lvgl/lv_micropython/tree/lvgl_javascript_v8#javascript-port

### Raspberry Pi Pico port

This port uses [Micropython infrastructure for C modules](https://docs.micropython.org/en/latest/develop/cmodules.html#compiling-the-cmodule-into-micropython) and `USER_C_MODULES` must be given:

```
cd ports/rp2
make USER_C_MODULES=../../lv_bindings/bindings.cmake
```

## Super Simple Example

First, LVGL needs to be imported and initialized

```python
import lvgl as lv
lv.init()
```

Then display driver and input driver needs to be registered.
Refer to [Porting the library](https://docs.lvgl.io/8.0/porting/index.html) for more information.
Here is an example of registering SDL drivers on Micropython unix port:

```python
import SDL
SDL.init()

# Register SDL display driver.

draw_buf = lv.disp_draw_buf_t()
buf1_1 = bytearray(480*10)
draw_buf.init(buf1_1, None, len(buf1_1)//4)
disp_drv = lv.disp_drv_t()
disp_drv.init()
disp_drv.draw_buf = draw_buf
disp_drv.flush_cb = SDL.monitor_flush
disp_drv.hor_res = 480
disp_drv.ver_res = 320
disp_drv.register()

# Regsiter SDL mouse driver

indev_drv = lv.indev_drv_t()
indev_drv.init()
indev_drv.type = lv.INDEV_TYPE.POINTER
indev_drv.read_cb = SDL.mouse_read
indev_drv.register()
```

Here is an alternative example, for registering ILI9341 drivers on Micropython ESP32 port:

```python
import lvgl as lv

# Import ILI9341 driver and initialized it

from ili9341 import ili9341
disp = ili9341()

# Import XPT2046 driver and initalize it

from xpt2046 import xpt2046
touch = xpt2046()
```

By default, both ILI9341 and XPT2046 are initialized on the same SPI bus with the following parameters:

- ILI9341: `miso=5, mosi=18, clk=19, cs=13, dc=12, rst=4, power=14, backlight=15, spihost=esp.HSPI_HOST, mhz=40, factor=4, hybrid=True`
- XPT2046: `cs=25, spihost=esp.HSPI_HOST, mhz=5, max_cmds=16, cal_x0 = 3783, cal_y0 = 3948, cal_x1 = 242, cal_y1 = 423, transpose = True, samples = 3`

You can change any of these parameters on ili9341/xpt2046 constructor.
You can also initalize them on different SPI buses if you want, by providing miso/mosi/clk parameters. Set them to -1 to use existing (initialized) spihost bus.

Now you can create the GUI itself:

```python

# Create a screen with a button and a label

scr = lv.obj()
btn = lv.btn(scr)
btn.align_to(lv.scr_act(), lv.ALIGN.CENTER, 0, 0)
label = lv.label(btn)
label.set_text("Hello World!")

# Load the screen

lv.scr_load(scr)

```

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
