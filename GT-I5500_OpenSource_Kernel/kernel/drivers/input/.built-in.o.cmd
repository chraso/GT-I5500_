cmd_drivers/input/built-in.o :=  ../toolchain/arm-eabi-4.4.0/bin/arm-eabi-ld -EL    -r -o drivers/input/built-in.o drivers/input/input-core.o drivers/input/evdev.o drivers/input/touchscreen/built-in.o drivers/input/misc/built-in.o 