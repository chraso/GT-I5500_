#ifndef _COMMON_H
#define _COMMON_H

#include <linux/kernel.h>
#include <linux/types.h>

#define GPIO_LEVEL_HIGH             1
#define GPIO_LEVEL_LOW              0

#define YES  1
#define NO  0

#if 1
#define trace_in()  debug( "%s +", __FUNCTION__ )
#define trace_out()  debug( "%s -", __FUNCTION__ )
#define trace_func()  debug( "%s called", __FUNCTION__ )
#else
#define trace_in()  
#define trace_out() 
#define trace_func()
#endif

#if 1
#define gprintk(fmt, x... ) printk("%s(%d): "fmt,__FUNCTION__,__LINE__,## x)
#define error(fmt,arg...) printk( KERN_CRIT "Si4709 E: " fmt "\n", ## arg)
#define debug(fmt,arg...) printk( KERN_CRIT "Si4709 D: " fmt "\n", ## arg)
#else
#define gprintk(x...)
#define error(fmt,arg...) 
#define debug(fmt,arg...) 
#endif

#endif

