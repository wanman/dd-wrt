/*
 * LZMA compressed kernel decompressor for bcm947xx boards
 *
 * Copyright (C) 2005 by Oleg I. Vdovikin <oleg@cs.msu.su>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *
 * Please note, this was code based on the bunzip2 decompressor code
 * by Manuel Novoa III  (mjn3@codepoet.org), although the only thing left
 * is an idea and part of original vendor code
 *
 *
 * 12-Mar-2005  Mineharu Takahara <mtakahar@yahoo.com>
 *   pass actual output size to decoder (stream mode
 *   compressed input is not a requirement anymore)
 *
 * 24-Apr-2005 Oleg I. Vdovikin
 *   reordered functions using lds script, removed forward decl
 *
 * ??-Nov-2005 Mike Baker
 *   reorder the script as an lzma wrapper; do not depend on flash access
 */

#include "LzmaDecode.h"

#define KSEG0			0x80000000
#define KSEG1			0xa0000000

#define KSEG1ADDR(a)		((((unsigned)(a)) & 0x1fffffffU) | KSEG1)

#define Index_Invalidate_I	0x00
#define Index_Writeback_Inv_D   0x01

#define cache_unroll(base,op)	\
	__asm__ __volatile__(		\
		".set noreorder;\n"		\
		".set mips3;\n"			\
		"cache %1, (%0);\n"		\
		".set mips0;\n"			\
		".set reorder\n"		\
		:						\
		: "r" (base),			\
		  "i" (op));


static __inline__ void blast_icache(unsigned long size, unsigned long lsize)
{
	unsigned long start = KSEG0;
	unsigned long end = (start + size);

	while(start < end) {
		cache_unroll(start,Index_Invalidate_I);
		start += lsize;
	}
}
static __inline__ void blast_dcache(unsigned long size, unsigned long lsize)
{
	unsigned long start = KSEG0;
	unsigned long end = (start + size);

	while(start < end) {
		cache_unroll(start,Index_Writeback_Inv_D);
		start += lsize;
	}
}

static unsigned char *data = 0xbff21000;
static inline unsigned char get_byte(void)
{
	static unsigned int vall;
	static unsigned inptr = 0;

	if (((unsigned int)inptr % 4) == 0) {
		vall = *(unsigned int *)data;
		data += 4;
	}
	return *(((unsigned char *)&vall) + (inptr++ & 3));
}


static int read_byte(void *object, const unsigned char **buffer, UInt32 * bufferSize)
{
	static unsigned char val;
	static unsigned int icnt = 0;
	*bufferSize = 1;
	val = get_byte();
	*buffer = &val;
	if (icnt++ % (1024 * 10) == 0)
		puts(".");
	return LZMA_RESULT_OK;
}


static char *buffer = (char *)0x80c00000;
unsigned char stack[8192];
unsigned long linux_args[3];
extern long bss_start[];
extern long bss_end[];

extern char lzma_start[];
extern char lzma_end[];

/* should be the first function */
void entry(unsigned long icache_size, unsigned long icache_lsize, 
	unsigned long dcache_size, unsigned long dcache_lsize)
{

	/* clear the bss */
	long *bssp;
	/* clear the bss */
	for (bssp = bss_start; bssp != bss_end; bssp++)
		*bssp = 0;
	unsigned int i;  /* temp value */
	unsigned int osize; /* uncompressed size */
	    
	ILzmaInCallback callback;
	CLzmaDecoderState vs;
	callback.Read = read_byte;
        puts("Atheros WiSOC DD-WRT LZMA Kernel Loader (");
	puts(__DATE__);
	puts(")\n");
        puts("decompressing");
	data = 0xbff21000;

	/* lzma args */
	i = get_byte();
	vs.Properties.lc = i % 9, i = i / 9;
	vs.Properties.lp = i % 5, vs.Properties.pb = i / 5;

	vs.Probs = (CProb *)buffer;

	for (i = 0; i < 4; i++)
		get_byte();

	osize = ((unsigned int)get_byte()) +
		((unsigned int)get_byte() << 8) +
		((unsigned int)get_byte() << 16) +
		((unsigned int)get_byte() << 24);

	for (i = 0; i < 4; i++) 
		get_byte();

	if ((i = LzmaDecode(&vs, &callback,
	(unsigned char*)KERNEL_ENTRY, osize, &osize)) == LZMA_RESULT_OK)
	{
    		puts("\ndone.\njump to kernel...\n");
		blast_dcache(dcache_size, dcache_lsize);
		blast_icache(icache_size, icache_lsize);

         	((void (*)(unsigned long, unsigned long, unsigned long)) KERNEL_ENTRY)
		(linux_args[0], linux_args[1], linux_args[2]);
	}
        puts("Fatal error while decompressing!\n");
}
