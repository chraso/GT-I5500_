cmd_drivers/leds/built-in.o :=  ../toolchain/arm-eabi-4.4.0/bin/arm-eabi-ld -EL    -r -o drivers/leds/built-in.o drivers/leds/led-core.o drivers/leds/led-class.o 
