cmd_drivers/fsr/fsr.o := ../toolchain/arm-eabi-4.4.0/bin/arm-eabi-ld -EL    -r -o drivers/fsr/fsr.o drivers/fsr/fsr_base.o drivers/fsr/bml_block.o drivers/fsr/bml_blkdev.o drivers/fsr/Core/BML/FSR_BML_RWInterface.o drivers/fsr/Core/BML/FSR_BML_BIFCommon.o drivers/fsr/Core/BML/FSR_BML_NonBlkMgr.o drivers/fsr/Core/BML/FSR_BML_BBMCommon.o drivers/fsr/Core/BML/FSR_BML_BBMMount.o drivers/fsr/Core/BML/FSR_BML_BadBlkMgr.o drivers/fsr/Core/BML/FSR_BML_Dump.o drivers/fsr/Core/BML/FSR_BML_ROInterface.o drivers/fsr/OAM/Linux/FSR_OAM_Linux.o drivers/fsr/OAM/Linux/FSR_SpinLock.o drivers/fsr/LLD/FlexOND/FSR_LLD_FlexOND.o drivers/fsr/LLD/OND/FSR_LLD_SWEcc.o drivers/fsr/LLD/OND/FSR_LLD_OneNAND.o drivers/fsr/LLD/OND/FSR_LLD_4K_OneNAND.o drivers/fsr/PAM/MSM7k/FSR_PAM_MSM7k.o drivers/fsr/PAM/MSM7k/FSR_PAM_asm.o drivers/fsr/Misc/FSR_Version.o drivers/fsr/Misc/FSR_DBG_Zone.o 
