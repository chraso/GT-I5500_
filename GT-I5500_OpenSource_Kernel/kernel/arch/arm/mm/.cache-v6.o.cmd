cmd_arch/arm/mm/cache-v6.o := ../toolchain/arm-eabi-4.4.0/bin/arm-eabi-gcc -Wp,-MD,arch/arm/mm/.cache-v6.o.d  -nostdinc -isystem /home/se-part/Opensource/Europa_TIM/eclair/GT-I5500_OpenSource/GT-I5500_OpenSource_Kernel/toolchain/arm-eabi-4.4.0/bin/../lib/gcc/arm-eabi/4.4.0/include -Iinclude  -I/home/se-part/Opensource/Europa_TIM/eclair/GT-I5500_OpenSource/GT-I5500_OpenSource_Kernel/kernel/arch/arm/include -include include/linux/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-msm/include -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=6 -march=armv6k -mtune=arm1136j-s -msoft-float     -c -o arch/arm/mm/cache-v6.o arch/arm/mm/cache-v6.S

deps_arch/arm/mm/cache-v6.o := \
  arch/arm/mm/cache-v6.S \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  /home/se-part/Opensource/Europa_TIM/eclair/GT-I5500_OpenSource/GT-I5500_OpenSource_Kernel/kernel/arch/arm/include/asm/linkage.h \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  /home/se-part/Opensource/Europa_TIM/eclair/GT-I5500_OpenSource/GT-I5500_OpenSource_Kernel/kernel/arch/arm/include/asm/assembler.h \
    $(wildcard include/config/cpu/feroceon.h) \
  /home/se-part/Opensource/Europa_TIM/eclair/GT-I5500_OpenSource/GT-I5500_OpenSource_Kernel/kernel/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/arm/thumb.h) \
    $(wildcard include/config/smp.h) \
  /home/se-part/Opensource/Europa_TIM/eclair/GT-I5500_OpenSource/GT-I5500_OpenSource_Kernel/kernel/arch/arm/include/asm/hwcap.h \
  arch/arm/mm/proc-macros.S \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/cpu/dcache/writethrough.h) \
  include/asm/asm-offsets.h \
  /home/se-part/Opensource/Europa_TIM/eclair/GT-I5500_OpenSource/GT-I5500_OpenSource_Kernel/kernel/arch/arm/include/asm/thread_info.h \
    $(wildcard include/config/arm/thumbee.h) \
  /home/se-part/Opensource/Europa_TIM/eclair/GT-I5500_OpenSource/GT-I5500_OpenSource_Kernel/kernel/arch/arm/include/asm/fpstate.h \
    $(wildcard include/config/vfpv3.h) \
    $(wildcard include/config/iwmmxt.h) \

arch/arm/mm/cache-v6.o: $(deps_arch/arm/mm/cache-v6.o)

$(deps_arch/arm/mm/cache-v6.o):
