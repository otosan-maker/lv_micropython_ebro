import uos
import os
from flashbdev import bdev


def check_bootsec():
    buf = bytearray(bdev.ioctl(5, 0))  # 5 is SEC_SIZE
    bdev.readblocks(0, buf)
    empty = True
    for b in buf:
        if b != 0xFF:
            empty = False
            break
    if empty:
        return True
    fs_corrupted()


def fs_corrupted():
    import time

    while 1:
        print(
            """\
The filesystem appears to be corrupted. If you had important data there, you
may want to make a flash snapshot to try to recover it. Otherwise, perform
factory reprogramming of MicroPython firmware (completely erase flash, followed
by firmware programming).
"""
        )
        time.sleep(3)


def setup():
    check_bootsec()
    print("Performing initial setup")
    uos.VfsLfs2.mkfs(bdev)
    vfs = uos.VfsLfs2(bdev)
    uos.mount(vfs, "/")
    os.mkdir("data")
    with open("/data/graf.txt", "w") as f:
        f.write(
            """\
{"Tmin": 0, "Xmax": 5, "function_y_t": "0.2*t*sin(t)", "function_x_t": "0.3*t*cos(t)", "parametric": 0, "Tmax": 15, "Ymax": 1.2, "Ymin": -1.5, "Xmin": -5, "function": "2*cos(x)"}
"""
        )
    with open("/data/wifi.txt", "w") as f:
        f.write(
            """\
{"network": "net", "startupInit": false, "password": "pass"}
"""
        )

    return vfs
