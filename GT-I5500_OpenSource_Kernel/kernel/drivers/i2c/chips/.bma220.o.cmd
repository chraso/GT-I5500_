cmd_drivers/i2c/chips/bma220.o := ../toolchain/arm-eabi-4.4.0/bin/arm-eabi-gcc -Wp,-MD,drivers/i2c/chips/.bma220.o.d  -nostdinc -isystem /home/se-part/Opensource/Europa_TIM/eclair/GT-I5500_OpenSource/GT-I5500_OpenSource_Kernel/toolchain/arm-eabi-4.4.0/bin/../lib/gcc/arm-eabi/4.4.0/include -Iinclude  -I/home/se-part/Opensource/Europa_TIM/eclair/GT-I5500_OpenSource/GT-I5500_OpenSource_Kernel/kernel/arch/arm/include -include include/linux/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-msm/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror -Werror-implicit-function-declaration -Os -marm -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=6 -march=armv6k -mtune=arm1136j-s -msoft-float -Uarm -Wframe-larger-than=2048 -fno-stack-protector -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wdeclaration-after-statement -Wno-pointer-sign -fwrapv -fno-dwarf2-cfi-asm  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(bma220)"  -D"KBUILD_MODNAME=KBUILD_STR(bma220)"  -c -o drivers/i2c/chips/bma220.o drivers/i2c/chips/bma220.c

deps_drivers/i2c/chips/bma220.o := \
  drivers/i2c/chips/bma220.c \
    $(wildcard include/config//reg.h) \
  include/linux/i2c/bma220.h \
    $(wildcard include/config/reg.h) \
    $(wildcard include/config//pos.h) \
    $(wildcard include/config//msk.h) \
    $(wildcard include/config//len.h) \

drivers/i2c/chips/bma220.o: $(deps_drivers/i2c/chips/bma220.o)

$(deps_drivers/i2c/chips/bma220.o):
