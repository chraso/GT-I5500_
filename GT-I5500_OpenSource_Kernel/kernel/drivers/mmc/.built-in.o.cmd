cmd_drivers/mmc/built-in.o :=  ../toolchain/arm-eabi-4.4.0/bin/arm-eabi-ld -EL    -r -o drivers/mmc/built-in.o drivers/mmc/core/built-in.o drivers/mmc/card/built-in.o drivers/mmc/host/built-in.o 
