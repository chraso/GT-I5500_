/*
 *---------------------------------------------------------------------------*
 *                                                                           *
 *          COPYRIGHT 2003-2009 SAMSUNG ELECTRONICS CO., LTD.                *
 *                          ALL RIGHTS RESERVED                              *
 *                                                                           *
 *   Permission is hereby granted to licensees of Samsung Electronics        *
 *   Co., Ltd. products to use or abstract this computer program only in     *
 *   accordance with the terms of the NAND FLASH MEMORY SOFTWARE LICENSE     *
 *   AGREEMENT for the sole purpose of implementing a product based on       *
 *   Samsung Electronics Co., Ltd. products. No other rights to reproduce,   *
 *   use, or disseminate this computer program, whether in part or in        *
 *   whole, are granted.                                                     *
 *                                                                           *
 *   Samsung Electronics Co., Ltd. makes no representation or warranties     *
 *   with respect to the performance of this computer program, and           *
 *   specifically disclaims any responsibility for any damages,              *
 *   special or consequential, connected with the use of this program.       *
 *                                                                           *
 *---------------------------------------------------------------------------*
*/
/**
 *  @version    RFS_3.0.0_b022-Nestle_1.0.0_b014-BTFS_1.0.1_b024-LinuStoreIII_1.2.0_b029-FSR_1.2.1_b125_RTM
 *  @file       drivers/fsr/debug.c
 *  @brief      This file is debug file module
 *
 */

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include "debug.h"

extern struct proc_dir_entry *fsr_proc_dir;







static int __init fsr_debug_init(void)
{


	return 0;

}

static void __exit fsr_debug_exit(void)
{

}

module_init(fsr_debug_init);
module_exit(fsr_debug_exit);

