/* Copyright (c) 2009, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 */
//bestiq TEST S5k4CA
#include <linux/delay.h>
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <media/msm_camera.h>
#include <mach/gpio.h>
#include "s5k4ca.h"

#include <mach/camera.h>//bestiq

#define SENSOR_DEBUG 0

struct s5k4ca_work {
	struct work_struct work;
};

static struct  s5k4ca_work *s5k4ca_sensorw;
static struct  i2c_client *s5k4ca_client;

struct s5k4ca_ctrl {
	const struct msm_camera_sensor_info *sensordata;
};


static struct s5k4ca_ctrl *s5k4ca_ctrl;

static DECLARE_WAIT_QUEUE_HEAD(s5k4ca_wait_queue);
DECLARE_MUTEX(s5k4ca_sem);
static int16_t s5k4ca_effect = CAMERA_EFFECT_OFF;

/*=============================================================
	EXTERNAL DECLARATIONS
==============================================================*/
extern struct s5k4ca_reg s5k4ca_regs;


/*=============================================================*/

#if 0//bestiq
static int s5k4ca_reset(const struct msm_camera_sensor_info *dev)
{
	int rc = 0;

	rc = gpio_request(dev->sensor_reset, "s5k4ca");

	if (!rc) {
		rc = gpio_direction_output(dev->sensor_reset, 0);
		mdelay(20);
		rc = gpio_direction_output(dev->sensor_reset, 1);
	}

	gpio_free(dev->sensor_reset);
	return rc;
}


static int32_t s5k4ca_i2c_txdata(unsigned short saddr,
	unsigned char *txdata, int length)
{
	struct i2c_msg msg[] = {
		{
			.addr = saddr,
			.flags = 0,
			.len = length,
			.buf = txdata,
		},
	};

#if SENSOR_DEBUG
	if (length == 2)
		CDBG("msm_io_i2c_w: 0x%04x 0x%04x\n",
			*(u16 *) txdata, *(u16 *) (txdata + 2));
	else if (length == 4)
		CDBG("msm_io_i2c_w: 0x%04x\n", *(u16 *) txdata);
	else
		CDBG("msm_io_i2c_w: length = %d\n", length);
#endif
	if (i2c_transfer(s5k4ca_client->adapter, msg, 1) < 0) {
		CDBG("s5k4ca_i2c_txdata failed\n");
		return -EIO;
	}

	return 0;
}

static int32_t s5k4ca_i2c_write(unsigned short saddr,
	unsigned short waddr, unsigned short wdata, enum s5k4ca_width width)
{
	int32_t rc = -EIO;
	unsigned char buf[4];

	memset(buf, 0, sizeof(buf));
	switch (width) {
	case WORD_LEN: {
		buf[0] = (waddr & 0xFF00)>>8;
		buf[1] = (waddr & 0x00FF);
		buf[2] = (wdata & 0xFF00)>>8;
		buf[3] = (wdata & 0x00FF);

		rc = s5k4ca_i2c_txdata(saddr, buf, 4);
	}
		break;

	case BYTE_LEN: {
		buf[0] = waddr;
		buf[1] = wdata;
		rc = s5k4ca_i2c_txdata(saddr, buf, 2);
	}
		break;

	default:
		break;
	}

	if (rc < 0)
		CDBG(
		"i2c_write failed, addr = 0x%x, val = 0x%x!\n",
		waddr, wdata);

	return rc;
}

static int32_t s5k4ca_i2c_write_table(
	struct s5k4ca_i2c_reg_conf const *reg_conf_tbl,
	int num_of_items_in_table)
{
	int i;
	int32_t rc = -EIO;

	for (i = 0; i < num_of_items_in_table; i++) {
		rc = s5k4ca_i2c_write(s5k4ca_client->addr,
			reg_conf_tbl->waddr, reg_conf_tbl->wdata,
			reg_conf_tbl->width);
		if (rc < 0)
			break;
		if (reg_conf_tbl->mdelay_time != 0)
			mdelay(reg_conf_tbl->mdelay_time);
		reg_conf_tbl++;
	}

	return rc;
}

static int s5k4ca_i2c_rxdata(unsigned short saddr,
	unsigned char *rxdata, int length)
{
	struct i2c_msg msgs[] = {
	{
		.addr   = saddr,
		.flags = 0,
		.len   = 2,
		.buf   = rxdata,
	},
	{
		.addr   = saddr,
		.flags = I2C_M_RD,
		.len   = length,
		.buf   = rxdata,
	},
	};

#if SENSOR_DEBUG
	if (length == 2)
		CDBG("msm_io_i2c_r: 0x%04x 0x%04x\n",
			*(u16 *) rxdata, *(u16 *) (rxdata + 2));
	else if (length == 4)
		CDBG("msm_io_i2c_r: 0x%04x\n", *(u16 *) rxdata);
	else
		CDBG("msm_io_i2c_r: length = %d\n", length);
#endif

	if (i2c_transfer(s5k4ca_client->adapter, msgs, 2) < 0) {
		CDBG("s5k4ca_i2c_rxdata failed!\n");
		return -EIO;
	}

	return 0;
}

static int32_t s5k4ca_i2c_read(unsigned short   saddr,
	unsigned short raddr, unsigned short *rdata, enum s5k4ca_width width)
{
	int32_t rc = 0;
	unsigned char buf[4];

	if (!rdata)
		return -EIO;

	memset(buf, 0, sizeof(buf));

	switch (width) {
	case WORD_LEN: {
		buf[0] = (raddr & 0xFF00)>>8;
		buf[1] = (raddr & 0x00FF);

		rc = s5k4ca_i2c_rxdata(saddr, buf, 2);
		if (rc < 0)
			return rc;

		*rdata = buf[0] << 8 | buf[1];
	}
		break;

	default:
		break;
	}

	if (rc < 0)
		CDBG("s5k4ca_i2c_read failed!\n");

	return rc;
}
#endif

#if 1//PGH
static int s5k4ca_sensor_read(unsigned short subaddr, unsigned short *data)
{
	int ret;
	unsigned char buf[2];
	struct i2c_msg msg = { s5k4ca_client->addr, 0, 2, buf };
	
	buf[0] = (subaddr >> 8);
	buf[1] = (subaddr & 0xFF);

	ret = i2c_transfer(s5k4ca_client->adapter, &msg, 1) == 1 ? 0 : -EIO;
	if (ret == -EIO) 
		goto error;

	msg.flags = I2C_M_RD;
	
	ret = i2c_transfer(s5k4ca_client->adapter, &msg, 1) == 1 ? 0 : -EIO;
	if (ret == -EIO) 
		goto error;

	*data = ((buf[0] << 8) | buf[1]);

error:
	return ret;
}
#endif//PGH

static int s5k4ca_sensor_write(unsigned short subaddr, unsigned short val)
{
	unsigned char buf[4];
	struct i2c_msg msg = { s5k4ca_client->addr, 0, 4, buf };

//	printk("[PGH] on write func s5k4ca_client->addr : %x\n", s5k4ca_client->addr);
//	printk("[PGH] on write func  s5k4ca_client->adapter->nr : %d\n", s5k4ca_client->adapter->nr);


	buf[0] = (subaddr >> 8);
	buf[1] = (subaddr & 0xFF);
	buf[2] = (val >> 8);
	buf[3] = (val & 0xFF);

	return i2c_transfer(s5k4ca_client->adapter, &msg, 1) == 1 ? 0 : -EIO;
}

static int s5k4ca_sensor_write_list(struct samsung_short_t *list,char *name)
{
	int i, ret;
	ret = 0;

	for (i = 0; list[i].subaddr != 0xffff; i++)
	{
		if(s5k4ca_sensor_write(list[i].subaddr, list[i].value) < 0)
			return -1;
	}
	return ret;
}

#if 0//PGH
static int32_t s5k4ca_set_lens_roll_off(void)
{
	int32_t rc = 0;
	rc = s5k4ca_i2c_write_table(&s5k4ca_regs.rftbl[0],
								 s5k4ca_regs.rftbl_size);
	return rc;
}
#endif//PGH

#if 0//PGH
static long s5k4ca_reg_init(void)
{
	int32_t array_length;
	int32_t i;
	long rc;

	/* PLL Setup Start */
	rc = s5k4ca_i2c_write_table(&s5k4ca_regs.plltbl[0],
					s5k4ca_regs.plltbl_size);

	if (rc < 0)
		return rc;
	/* PLL Setup End   */

	array_length = s5k4ca_regs.prev_snap_reg_settings_size;

	/* Configure sensor for Preview mode and Snapshot mode */
	for (i = 0; i < array_length; i++) {
		rc = s5k4ca_i2c_write(s5k4ca_client->addr,
		  s5k4ca_regs.prev_snap_reg_settings[i].register_address,
		  s5k4ca_regs.prev_snap_reg_settings[i].register_value,
		  WORD_LEN);

		if (rc < 0)
			return rc;
	}

	/* Configure for Noise Reduction, Saturation and Aperture Correction */
	array_length = s5k4ca_regs.noise_reduction_reg_settings_size;

	for (i = 0; i < array_length; i++) {
		rc = s5k4ca_i2c_write(s5k4ca_client->addr,
			s5k4ca_regs.noise_reduction_reg_settings[i].register_address,
			s5k4ca_regs.noise_reduction_reg_settings[i].register_value,
			WORD_LEN);

		if (rc < 0)
			return rc;
	}

	/* Set Color Kill Saturation point to optimum value */
	rc =
	s5k4ca_i2c_write(s5k4ca_client->addr,
	0x35A4,
	0x0593,
	WORD_LEN);
	if (rc < 0)
		return rc;

	rc = s5k4ca_i2c_write_table(&s5k4ca_regs.stbl[0],
					s5k4ca_regs.stbl_size);
	if (rc < 0)
		return rc;

	rc = s5k4ca_set_lens_roll_off();
	if (rc < 0)
		return rc;

	return 0;
}
#endif//PGH

static long s5k4ca_set_effect(int mode, int effect)
{
	uint16_t reg_addr;
	//uint16_t reg_val;
	long rc = 0;

	switch (mode) {
	case SENSOR_PREVIEW_MODE:
		printk("[PGH] %s SENSOR_PREVIEW_MODE\n", __func__);
		/* Context A Special Effects */
		reg_addr = 0x2799;
		break;

	case SENSOR_SNAPSHOT_MODE:
		printk("[PGH] %s SENSOR_SNAPSHOT_MODE\n", __func__);
		/* Context B Special Effects */
		reg_addr = 0x279B;
		break;

	default:
		printk("[PGH] %s default\n", __func__);
		reg_addr = 0x2799;
		break;
	}

	switch (effect) {
	case CAMERA_EFFECT_OFF: {
	printk("[PGH] CAMERA_EFFECT_OFF\n");
	/*
		reg_val = 0x6440;

		rc = s5k4ca_i2c_write(s5k4ca_client->addr,
			0x338C, reg_addr, WORD_LEN);
		if (rc < 0)
			return rc;

		rc = s5k4ca_i2c_write(s5k4ca_client->addr,
			0x3390, reg_val, WORD_LEN);
		if (rc < 0)
			return rc;
	*/
	}
			break;

	case CAMERA_EFFECT_MONO: {
	printk("[PGH] CAMERA_EFFECT_MONO\n");
	/*
		reg_val = 0x6441;
		rc = s5k4ca_i2c_write(s5k4ca_client->addr,
			0x338C, reg_addr, WORD_LEN);
		if (rc < 0)
			return rc;

		rc = s5k4ca_i2c_write(s5k4ca_client->addr,
			0x3390, reg_val, WORD_LEN);
		if (rc < 0)
			return rc;
	*/
	}
		break;

	case CAMERA_EFFECT_NEGATIVE: {
	printk("[PGH] CAMERA_EFFECT_NEGATIVE\n");
	/*
		reg_val = 0x6443;
		rc = s5k4ca_i2c_write(s5k4ca_client->addr,
			0x338C, reg_addr, WORD_LEN);
		if (rc < 0)
			return rc;

		rc = s5k4ca_i2c_write(s5k4ca_client->addr,
			0x3390, reg_val, WORD_LEN);
		if (rc < 0)
			return rc;
	*/
	}
		break;

	case CAMERA_EFFECT_SOLARIZE: {

	printk("[PGH] CAMERA_EFFECT_SOLARIZE\n");
	/*
		reg_val = 0x6445;
		rc = s5k4ca_i2c_write(s5k4ca_client->addr,
			0x338C, reg_addr, WORD_LEN);
		if (rc < 0)
			return rc;

		rc = s5k4ca_i2c_write(s5k4ca_client->addr,
			0x3390, reg_val, WORD_LEN);
		if (rc < 0)
			return rc;
	*/
	}
		break;

	case CAMERA_EFFECT_SEPIA: {
	printk("[PGH] CAMERA_EFFECT_SEPIA\n");
	/*
		reg_val = 0x6442;
		rc = s5k4ca_i2c_write(s5k4ca_client->addr,
			0x338C, reg_addr, WORD_LEN);
		if (rc < 0)
			return rc;

		rc = s5k4ca_i2c_write(s5k4ca_client->addr,
			0x3390, reg_val, WORD_LEN);
		if (rc < 0)
			return rc;
	*/
	}
		break;

	default: {

	printk("[PGH] default .dsfsdf\n");
	/*
		reg_val = 0x6440;
		rc = s5k4ca_i2c_write(s5k4ca_client->addr,
			0x338C, reg_addr, WORD_LEN);
		if (rc < 0)
			return rc;

		rc = s5k4ca_i2c_write(s5k4ca_client->addr,
			0x3390, reg_val, WORD_LEN);
		if (rc < 0)
			return rc;
	*/

		return -EINVAL;
	}
	}
	s5k4ca_effect = effect;
	/* Refresh Sequencer */
/*
	rc = s5k4ca_i2c_write(s5k4ca_client->addr,
		0x338C, 0xA103, WORD_LEN);
	if (rc < 0)
		return rc;

	rc = s5k4ca_i2c_write(s5k4ca_client->addr,
		0x3390, 0x0005, WORD_LEN);
*/
	return rc;
}

static long s5k4ca_set_sensor_mode(int mode)
{
	//long rc = 0;

	printk("[CAM-SENSOR] =Sensor Mode ");
	switch (mode) {
	case SENSOR_PREVIEW_MODE:
		printk("[PGH]-> Preview \n");
		s5k4ca_sensor_write_list(s5k4ca_preview,"s5k4ca_preview"); // preview start
		mdelay(100);		
		
		break;

	case SENSOR_SNAPSHOT_MODE:
		printk("[PGH}-> Capture \n");		
		s5k4ca_sensor_write(0xFCFC, 0xD000);	
		s5k4ca_sensor_write(0x002C, 0x7000);
		s5k4ca_sensor_write(0x002E, 0x12FE);	
		msleep(100);		

		s5k4ca_sensor_write_list(s5k4ca_snapshot_normal,"s5k4ca_snapshot_normal");
		msleep(200);
		break;

	case SENSOR_RAW_SNAPSHOT_MODE:

		printk("[PGH}-> Capture RAW \n");		
		break;

	default:
		return -EINVAL;
	}

	return 0;
}

static int s5k4ca_sensor_init_probe(const struct msm_camera_sensor_info *data)
{
	int rc = 0;

	unsigned short	id = 0; //PGH FOR TEST
	printk("[PGH] TEST I2C COMMAND PGH~~\n");

	s5k4ca_sensor_write(0x002C, 0x7000);
	s5k4ca_sensor_write(0x002E, 0x01FA);
	s5k4ca_sensor_read(0x0F12, &id);
	printk("[PGH] SENSOR FW => id 0x%04x \n", id);



	printk("[PGH]  111111 %s/%d \n", __func__, __LINE__);	

	s5k4ca_sensor_write_list(s5k4ca_init0,"s5k4ca_init0");

	msleep(100);	

	printk("[PGH]  22222 %s/%d \n", __func__, __LINE__);	

	s5k4ca_sensor_write_list(s5k4ca_init1,"s5k4ca_init1");

	return rc;

}

int s5k4ca_sensor_init(const struct msm_camera_sensor_info *data)
{
	int rc = 0;
	int test_value =0; //PGH FOR TEST

	printk("[PGH]  11111 %s/%d \n", __func__, __LINE__);	

	printk("[PGH] using other gpio funcs\n");
	gpio_tlmm_config(GPIO_CFG(0, 0,GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), GPIO_ENABLE); //RESET
	gpio_tlmm_config(GPIO_CFG(1, 0,GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), GPIO_ENABLE); //STANDBY
	gpio_tlmm_config(GPIO_CFG(107, 0,GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), GPIO_ENABLE); //CAM_EN
 
	gpio_set_value(0, 0);
	gpio_set_value(1, 0);
	gpio_set_value(107, 0);


	gpio_tlmm_config(GPIO_CFG(2, 0,GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), GPIO_ENABLE); //RESET
	gpio_tlmm_config(GPIO_CFG(3, 0,GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), GPIO_ENABLE); //STANDBY

	gpio_set_value(2, 0);
	gpio_set_value(3, 0);

	test_value = gpio_get_value(2);
	printk("[PGH] CIF STANDBY  : %d\n",test_value);	

	test_value = gpio_get_value(3);
	printk("[PGH] CIF RESET  : %d\n",test_value);	



	s5k4ca_ctrl = kzalloc(sizeof(struct s5k4ca_ctrl), GFP_KERNEL);
	if (!s5k4ca_ctrl) {
		CDBG("s5k4ca_init failed!\n");
		rc = -ENOMEM;
		goto init_done;
	}

	if (data)
		s5k4ca_ctrl->sensordata = data;


    test_value = gpio_get_value(1);
    printk("[PGH] 1 STANDBY  : %d\n",test_value);	

#if 1//PGH
	gpio_set_value(1, 1);
#else//PGH2
	rc =  gpio_direction_output(1, 1); //Standby high
	if(rc < 0){
		CDBG("Standby failed!\n");
		goto init_fail;
	}
#endif//PGH
    test_value = gpio_get_value(1);
    printk("[PGH] 2 STANDBY  : %d\n",test_value);	
	

	msleep(1);


    test_value = gpio_get_value(107);
    printk("[PGH] 1 CAM_EN  : %d\n",test_value);	

	gpio_set_value(107, 1);

/*	
	rc =  gpio_direction_output(107, 1); //CAM_EN high
	if(rc < 0){
		CDBG("CAM_EN failed!\n");
		goto init_fail;
		
	}
*/
    test_value = gpio_get_value(107);
    printk("[PGH] 2 CAM_EN  : %d\n",test_value);	


	msleep(1);

	/* Input MCLK = 24MHz */
	msm_camio_clk_rate_set(24000000);
	msleep(5);

	msm_camio_camif_pad_reg_reset();

	msleep(1);
	

    test_value = gpio_get_value(0);
    printk("[PGH] 1 RESET  : %d\n",test_value);	
	

	gpio_set_value(0, 1);

/*	
	rc =  gpio_direction_output(0, 1); //Reset high
	if(rc < 0){
		CDBG("reset failed!\n");
		goto init_fail;
	}
*/
    test_value = gpio_get_value(0);
    printk("[PGH] 2 RESET  : %d\n",test_value);	
	
	

	msleep(40);

//I2C HIGH
	printk("[PGH]  22222  I2C HIGH  %s/%d \n", __func__, __LINE__);	

	gpio_set_value(60, 1);
	gpio_set_value(61, 1);	
	msleep(40);	

	rc = s5k4ca_sensor_init_probe(data);
	if (rc < 0) {
		CDBG("s5k4ca_sensor_init failed!\n");
		goto init_fail;
	}
	printk("[PGH]  3333333 %s/%d \n", __func__, __LINE__);	


init_done:
	return rc;

init_fail:
	kfree(s5k4ca_ctrl);
	return rc;
}

static int s5k4ca_init_client(struct i2c_client *client)
{
	/* Initialize the MSM_CAMI2C Chip */
	init_waitqueue_head(&s5k4ca_wait_queue);
	return 0;
}

int s5k4ca_sensor_config(void __user *argp)
{
	struct sensor_cfg_data cfg_data;
	long   rc = 0;

	if (copy_from_user(&cfg_data,
			(void *)argp,
			sizeof(struct sensor_cfg_data)))
		return -EFAULT;

	/* down(&s5k4ca_sem); */

	CDBG("s5k4ca_ioctl, cfgtype = %d, mode = %d\n",
		cfg_data.cfgtype, cfg_data.mode);

		switch (cfg_data.cfgtype) {
		case CFG_SET_MODE:
			rc = s5k4ca_set_sensor_mode(
						cfg_data.mode);
			break;

		case CFG_SET_EFFECT:
			rc = s5k4ca_set_effect(cfg_data.mode,
						cfg_data.cfg.effect);
			break;

		case CFG_GET_AF_MAX_STEPS:
		default:
			rc = -EINVAL;
			break;
		}

	/* up(&s5k4ca_sem); */

	return rc;
}

int s5k4ca_sensor_release(void)
{
	int rc = 0;

	/* down(&s5k4ca_sem); */

	kfree(s5k4ca_ctrl);
	/* up(&s5k4ca_sem); */

	return rc;
}

static int s5k4ca_i2c_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	int rc = 0;
	printk("[PGH]  11111 %s/%d \n", __func__, __LINE__);	

	
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		rc = -ENOTSUPP;
		goto probe_failure;
	}

	s5k4ca_sensorw =
		kzalloc(sizeof(struct s5k4ca_work), GFP_KERNEL);

	if (!s5k4ca_sensorw) {
		rc = -ENOMEM;
		goto probe_failure;
	}

	i2c_set_clientdata(client, s5k4ca_sensorw);
	s5k4ca_init_client(client);
	s5k4ca_client = client;

	printk("[PGH] s5k4ca_probe succeeded!  %s/%d \n", __func__, __LINE__);	

	CDBG("s5k4ca_probe succeeded!\n");

	return 0;

probe_failure:
	kfree(s5k4ca_sensorw);
	s5k4ca_sensorw = NULL;
	CDBG("s5k4ca_probe failed!\n");
	return rc;
}

static const struct i2c_device_id s5k4ca_i2c_id[] = {
	{ "s5k4ca", 0},
	{ },
};

static struct i2c_driver s5k4ca_i2c_driver = {
	.id_table = s5k4ca_i2c_id,
	.probe  = s5k4ca_i2c_probe,
	.remove = __exit_p(s5k4ca_i2c_remove),
	.driver = {
		.name = "s5k4ca",
	},
};

static int s5k4ca_sensor_probe(const struct msm_camera_sensor_info *info,
				struct msm_sensor_ctrl *s)
{

	int rc = i2c_add_driver(&s5k4ca_i2c_driver);
	if (rc < 0 || s5k4ca_client == NULL) {
		rc = -ENOTSUPP;
		goto probe_done;
	}
	printk("[PGH]  11111 %s/%d \n", __func__, __LINE__);	
	printk("[PGH]  22222 %s/%d \n", __func__, __LINE__);	



	printk("[PGH] s5k4ca_client->addr : %x\n", s5k4ca_client->addr);
	printk("[PGH] s5k4ca_client->adapter->nr : %d\n", s5k4ca_client->adapter->nr);



	/* Input MCLK = 24MHz */
	msm_camio_clk_rate_set(24000000);
	mdelay(5);

#if 0//bestiq
	rc = s5k4ca_sensor_init_probe(info);
	if (rc < 0)
		goto probe_done;
#endif

	s->s_init = s5k4ca_sensor_init;
	s->s_release = s5k4ca_sensor_release;
	s->s_config  = s5k4ca_sensor_config;

probe_done:
	CDBG("%s %s:%d\n", __FILE__, __func__, __LINE__);
	return rc;
}

static int __s5k4ca_probe(struct platform_device *pdev)
{
	printk("[PGH]  %s/%d \n", __func__, __LINE__);	
	return msm_camera_drv_start(pdev, s5k4ca_sensor_probe);
}

static struct platform_driver msm_camera_driver = {
	.probe = __s5k4ca_probe,
	.driver = {
		.name = "msm_camera_s5k4ca",
		.owner = THIS_MODULE,
	},
};

static int __init s5k4ca_init(void)
{
	printk("[PGH]  %s/%d \n", __func__, __LINE__);
	return platform_driver_register(&msm_camera_driver);
}

module_init(s5k4ca_init);
