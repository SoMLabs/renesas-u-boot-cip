/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2023 Somlabs
 */

#ifndef __VISIONSOM_X2L_H
#define __VISIONSOM_X2L_H

#include <asm/arch/rmobile.h>

#define CONFIG_REMAKE_ELF

#ifdef CONFIG_SPL
#define CONFIG_SPL_TARGET	"spl/u-boot-spl.scif"
#endif

/* boot option */

#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG

/* Generic Interrupt Controller Definitions */
/* RZ/G2L and RZ/V2L use GIC-v3 */
#define CONFIG_GICV3
#define GICD_BASE	0x11900000
#define GICR_BASE	0x11960000

/* console */
#define CONFIG_SYS_CBSIZE		2048
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE
#define CONFIG_SYS_MAXARGS		64
#define CONFIG_SYS_BAUDRATE_TABLE	{ 115200, 38400 }

/* PHY needs a longer autoneg timeout */
#define PHY_ANEG_TIMEOUT		20000

/* MEMORY */
#define CONFIG_SYS_INIT_SP_ADDR		CONFIG_SYS_TEXT_BASE

/* SDHI clock freq */
#define CONFIG_SH_SDHI_FREQ		133000000

#define DRAM_RSV_SIZE			0x08000000
#define CONFIG_SYS_SDRAM_BASE		(0x40000000 + DRAM_RSV_SIZE)
#define CONFIG_SYS_SDRAM_SIZE		(0x40000000u - DRAM_RSV_SIZE) //total 2GB
#define CONFIG_SYS_LOAD_ADDR		0x58000000
#define CONFIG_LOADADDR			CONFIG_SYS_LOAD_ADDR // Default load address for tfpt,bootp...
#define CONFIG_VERY_BIG_RAM
#define CONFIG_MAX_MEM_MAPPED		(0x40000000u - DRAM_RSV_SIZE)

#define CONFIG_SYS_MONITOR_BASE		0x00000000
#define CONFIG_SYS_MONITOR_LEN		(1 * 1024 * 1024)
#define CONFIG_SYS_MALLOC_LEN		(64 * 1024 * 1024)
#define CONFIG_SYS_BOOTM_LEN		(64 << 20)

/* The HF/QSPI layout permits up to 1 MiB large bootloader blob */
#define CONFIG_BOARD_SIZE_LIMIT		1048576

/* ENV setting */

#ifdef CONFIG_R9A07G044L
#define SOC_TYPE "g2l"
#elif CONFIG_R9A07G054L
#define SOC_TYPE "v2l"
#else
#error "Wrong SOC type"
#endif

#define USB_FLASHER_CMD \
	"setenv soc " SOC_TYPE "; " \
	"if usb start; then " \
		"mmc dev 0; " \
		"if load usb 0 $loadaddr somlabs-image-visionsom-${soc}-cb.wic; then " \
			"setexpr writesize ${filesize} / 0x200; " \
			"mmc write $loadaddr 0 $writesize; " \
		"elif load usb 0 $loadaddr somlabs-image-visionsom-${soc}-cb.simg; then " \
			"mmc swrite $loadaddr 0; " \
		"fi; " \
		"if load usb 0 $loadaddr fip-visionsom-${soc}-cb.bin; then " \
			"setexpr writesize ${filesize} / 0x200; " \
			"mmc dev 0 1; " \
			"mmc write $loadaddr 0x100 $writesize; " \
		"fi; " \
		"if load usb 0 $loadaddr bl2_bp-visionsom-${soc}-cb.bin; then " \
			"setexpr writesize ${filesize} / 0x200; " \
			"mmc dev 0 1; " \
			"mmc write $loadaddr 0x1 $writesize; " \
		"fi; " \
	"fi; "

#define CONFIG_EXTRA_ENV_SETTINGS \
	"usb_pgood_delay=2000\0" \
	"bootm_size=0x10000000 \0" \
	"emmcbootargs=setenv bootargs earlycon root=/dev/mmcblk0p1 rw rootwait \0" \
	"usbbootargs=setenv bootargs earlycon root=/dev/sda1 rw rootwait \0" \
	"bootimage=booti 0x48080000 - 0x48000000 \0" \
	"emmcload=ext4load mmc 0:1 0x48080000 boot/Image; ext4load mmc 0:1 0x48000000 boot/" CONFIG_DEFAULT_FDT_FILE "\0" \
	"usbflash=" USB_FLASHER_CMD "reset; \0"

#define CONFIG_BOOTCOMMAND	"env default -a; "\
							"if run emmcload; then " \
								"run emmcbootargs; run bootimage; " \
							"else " \
								"run usbflash; " \
							"fi"

/* For board */
/* Ethernet RAVB */
#define CONFIG_BITBANGMII_MULTI

#endif /* __VISIONSOM_X2L_H */
