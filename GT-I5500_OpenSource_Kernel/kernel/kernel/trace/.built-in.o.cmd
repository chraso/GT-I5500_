cmd_kernel/trace/built-in.o :=  ../toolchain/arm-eabi-4.4.0/bin/arm-eabi-ld -EL    -r -o kernel/trace/built-in.o kernel/trace/ring_buffer.o kernel/trace/trace.o kernel/trace/trace_nop.o 
