/*
 *  Platform depdenent initialization
 *  
 *  Copyright (C) 2009 Faraday Corp. (http://www.faraday-tech.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <linux/platform_device.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <asm/setup.h>
#include <linux/module.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach-types.h>
#include <asm/mach/time.h>
#include <mach/spec.h>
#include <mach/ftintc010.h>
#include <mach/fttmr010.h>
#include <mach/platform/pmu.h>
#include <mach/ftpmu010.h>

static struct meminfo   mem_info;
/******************************************************************************
 * platform devices
 * All IP module resource defined here
 *****************************************************************************/

/******************************************************************************
 * I2C devices
 *****************************************************************************/
/* i2c:0 */ 
static struct resource ftiic010_0_resources[] = {
	{
		.start  = I2C_FTI2C010_0_PA_BASE,
		.end    = I2C_FTI2C010_0_PA_LIMIT,
		.flags  = IORESOURCE_MEM,
	},
	{
		.start	= I2C_FTI2C010_0_IRQ,
		.end	= I2C_FTI2C010_0_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device ftiic010_0_device = {
	.name		= "ftiic010",
	.id		    = 0,
	.num_resources	= ARRAY_SIZE(ftiic010_0_resources),
	.resource	= ftiic010_0_resources,
};

/*GPIO 0*/
static struct resource ftgpio010_0_resource[] = {
	{
		.start	= GPIO_FTGPIO010_PA_BASE,
		.end 	= GPIO_FTGPIO010_PA_LIMIT,
		.flags  = IORESOURCE_MEM
	},
	{
		.start	= GPIO_FTGPIO010_0_IRQ,
		.end	= GPIO_FTGPIO010_0_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device ftgpio010_0_device = {
	.name	= "ftgpio010",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(ftgpio010_0_resource),
	.resource	= ftgpio010_0_resource
};

/*GPIO 1*/
static struct resource ftgpio010_1_resource[] = {
	{
		.start	= GPIO_FTGPIO010_1_PA_BASE,
		.end 	= GPIO_FTGPIO010_1_PA_LIMIT,
		.flags  = IORESOURCE_MEM
	},
	{
		.start	= GPIO_FTGPIO010_1_IRQ,
		.end	= GPIO_FTGPIO010_1_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device ftgpio010_1_device = {
	.name	= "ftgpio010",
	.id		= 1,
	.num_resources	= ARRAY_SIZE(ftgpio010_1_resource),
	.resource	= ftgpio010_1_resource
};

/*GPIO 2 */
static struct resource ftgpio010_2_resource[] = {
	{
		.start	= GPIO_FTGPIO010_2_PA_BASE,
		.end 	= GPIO_FTGPIO010_2_PA_LIMIT,
		.flags  = IORESOURCE_MEM
	},
	{
		.start	= GPIO_FTGPIO010_2_IRQ,
		.end	= GPIO_FTGPIO010_2_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device ftgpio010_2_device = {
	.name	= "ftgpio010",
	.id		= 2,
	.num_resources	= ARRAY_SIZE(ftgpio010_2_resource),
	.resource	= ftgpio010_2_resource
};

/* SPI 0 */
static struct resource ftssp010_spi_resources[] = {
    {
        .start  = SSP_FTSSP010_0_PA_BASE,
        .end    = SSP_FTSSP010_0_PA_LIMIT,
        .flags  = IORESOURCE_MEM,
    }, {
        .start  = SSP_FTSSP010_0_IRQ,
        .end    = SSP_FTSSP010_0_IRQ,
        .flags  = IORESOURCE_IRQ,
    },
};

static struct platform_device ftssp010_spi_device = {
    .name       = "ftssp010_spi",
    .id     = 0,
    .num_resources  = ARRAY_SIZE(ftssp010_spi_resources),
    .resource   = ftssp010_spi_resources,
};

/* ****************************************************************************
 * array contains all platform devices
 * ****************************************************************************/ 
static struct platform_device *gm_devices[] __initdata = 
{    
	/* I2C */
	&ftiic010_0_device,
	/* GPIO */
	&ftgpio010_0_device,
	&ftgpio010_1_device,
	&ftgpio010_2_device,
    /* SPI */
    &ftssp010_spi_device,
};

/******************************************************************************
 * timer - clockevent and clocksource
 *****************************************************************************/
static struct fttmr010_clockevent fttmr010_0_clockevent = {
	.clockevent = {
		.name	= "fttmr010:0",
		.irq	= TIMER_FTTMR010_0_IRQ0,
	},
	.base	= (void __iomem *)TIMER_FTTMR010_0_VA_BASE,
	.id	= 0,
};

static struct fttmr010_clocksource fttmr010_1_clocksource = {
	.clocksource = {	
		.name	= "fttmr010:1",
	},
	.base	= (void __iomem *)TIMER_FTTMR010_0_VA_BASE,
	.id	= 1,
};

static void __init gm_sys_timer_init(void)
{
	unsigned int pclk = pmu_get_apb_clk();
    
	fttmr010_0_clockevent.freq = pclk;
	fttmr010_clockevent_init(&fttmr010_0_clockevent);

	fttmr010_1_clocksource.freq = pclk;
	fttmr010_clocksource_init(&fttmr010_1_clocksource);
}

struct sys_timer gm_sys_timer = {
	.init	= gm_sys_timer_init,
};

/******************************************************************************
 * platform dependent functions
 *****************************************************************************/
extern const struct map_desc platform_io_desc[PLATFORM_IO_DESC_NR];

static void __init platform_map_io(void)
{        
    iotable_init((struct map_desc*)platform_io_desc, ARRAY_SIZE(platform_io_desc));
}

void __iomem *ftintc010_base_addr;

static void __init gm_init_irq(void)
{
	struct ftintc010_trigger_type master_trigger_type = {
		.irqmode	= PLATFORM_IRQ_TRIGGER_MODE2,
		.irqlevel	= ~PLATFORM_IRQ_TRIGGER_LEVEL2,
		.fiqmode	= PLATFORM_FIQ_TRIGGER_MODE2,
		.fiqlevel	= ~PLATFORM_FIQ_TRIGGER_LEVEL2,
#ifdef CONFIG_FTINTC010EX
        .irqmodeex	= PLATFORM_IRQ_TRIGGER_MODEEX2,
		.irqlevelex	= ~PLATFORM_IRQ_TRIGGER_LEVELEX2,
		.fiqmodeex	= PLATFORM_FIQ_TRIGGER_MODEEX2,
		.fiqlevelex	= ~PLATFORM_FIQ_TRIGGER_LEVELEX2,
#endif
	};

	/*
	 * initialize primary interrupt controller
	 */
	ftintc010_base_addr = __io(INTC_FTINTC010_0_VA_BASE);
	ftintc010_init(0, ftintc010_base_addr, 0, &master_trigger_type);
}

static struct mtd_partition partitions[] = {
    {
     .name = "Linux Section",           // for system usage
     .offset = 0xD6000,
#if 0
     .size = 0xDC0000           // 13.75MB
#else
     .size = 0xFC0000 //15.75MB
#endif
     },
    {
     .name = "User Section",            // free for use
#if 0
     .offset = 0xE96000,
#else
     .offset = 0x1E56000,
#endif
     .size = MTDPART_SIZ_FULL},
    {
     .name = "Loader Section",
     .offset = 0x1000,
     .size = 0x5000},
    {
     .name = "BurnIn Section",          // for system usage
     .offset = 0x6000,          // Burn-in ,start addr MTD_PA_BASE
     .size = 0xA0000},
    {
     .name = "UBoot Section",           // for system usage
     .offset = 0xA6000,         // start addr MTD_PA_BASE + 0x200000
     .size = 0x30000},
    {
     .name = "CFG Section",           // for MAC usage
     .offset = 0x6E0000,         
     .size = 0x20000},  
};

static struct flash_platform_data spi_flash_platform_data = {
    .name = "wb_spi_flash",
    .parts = partitions,
    .nr_parts = ARRAY_SIZE(partitions)
};

static struct spi_board_info aux_spi_flash = {
        .modalias = "WINBOND_SPI_FLASH",
        .max_speed_hz = 50 * 1000 * 1000,   //50Mhz
        .bus_num = 0,               //on bus 0
        .chip_select = 1,           //second chip select
        .mode = SPI_MODE_0,
};

static struct spi_board_info spi_devs_info[] __initdata = {
    {
        .modalias = "WINBOND_SPI_FLASH",
        .max_speed_hz = 50 * 1000 * 1000,   //50Mhz
        .bus_num = 0,               //on bus 0
        .chip_select = 0,           //first chip select
        .mode = SPI_MODE_0,
        .platform_data = &spi_flash_platform_data,//MTD partion
        .controller_data = &aux_spi_flash//second SPI flash info
    },
};

static void __init gm_board_init(void)
{    
    /* will invoke platform_device_register() to register all platform devices 
     */
	platform_add_devices(gm_devices, ARRAY_SIZE(gm_devices));
	
    /* add spi device here, when add corresponding spi driver, they will bind together
    */
    spi_register_board_info(spi_devs_info, ARRAY_SIZE(spi_devs_info));
}

/******************************************************************************
 * store memory information in parsing command line stage
 *****************************************************************************/
static inline void parse_early_mem(char **p)
{
	unsigned long size, start, order;
    int nr_bank;
    
	start = PHYS_OFFSET;
	size  = memparse(*p, p);
	/* This prevent some users reserved some memory space and cause the size is not 2^X */
	order = get_order(size);
	size = PAGE_SIZE << order;
	
	if (**p == '@')
		start = memparse(*p + 1, p);
    
    nr_bank = mem_info.nr_banks;
    mem_info.bank[nr_bank].start = start;
    mem_info.bank[nr_bank].size  = size;
    mem_info.bank[nr_bank].node = nr_bank;
    mem_info.nr_banks ++;
}

static void __init platform_fixup_memory(struct machine_desc *desc, struct tag *tag,
                                         char **cmdline, struct meminfo *mi)
{
    char key[] = "mem=", *from;
    int  i;
    
    memset(&mem_info, 0, sizeof(mem_info));    
    from = *cmdline;
    
    for (i = 0; i < strlen(*cmdline) - strlen(key); i ++)
	{
		if (memcmp(from, key, strlen(key)) == 0)
		{
			from += strlen(key);
			parse_early_mem(&from);
		}
		else
			from ++;
	}
}

void platform_get_memory(struct meminfo **p_memory)
{
    *p_memory = &mem_info;
    return;
}

MACHINE_START(GM, PLATFORM_NAME)
	.phys_io	    = PLATFORM_BOOTTIME_IO_PA_BASE,
	.io_pg_offst	= ((PLATFORM_BOOTTIME_IO_VA_BASE) >> 20) << 2,
	.boot_params	= BOOT_PARAMETER_PA_BASE,
	.map_io		    = platform_map_io,
	.init_irq	    = gm_init_irq,
	.timer		    = &gm_sys_timer,
	.fixup = platform_fixup_memory,
	.init_machine	= gm_board_init,
MACHINE_END
