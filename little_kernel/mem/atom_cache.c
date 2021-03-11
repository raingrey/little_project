#include <stdlib.h>
#include <stdio.h>
#include "mem.h"

#define NEXT_PAGE_ADDR_MASK (~((1 << 9) - 1))
#define NR_FREE_UNIT_IN_NEXT_PAGE_ADDR_MASK ((1 << 9) - 1)

s32 free_mem_node_atom_cache(struct atom_cache_desc *acd, void *p)
{
	void *pg, *next;
	u64 *free_bit;
	s32 offset, nr_free;

	/* clean free unit number in next_node address */
	pg = p & (~(1 << (acd->used_mem_node_weight + 1) - 1));
	/* get unit addr offset in this mem_node */
	offset = p & (1 << (acd->used_mem_node_weight + 1) - 1);
	/* get free unit number in address */
	nr_free = p & NR_FREE_UNIT_IN_NEXT_PAGE_ADDR_MASK;
	/* highway */
	if ((pg & NR_FREE_UNIT_IN_NEXT_PAGE_ADDR_MASK) > 0) {
		free_bit = pg + 1;
		bit_num = (offset - free_bit) >> (1 << acd->atom_size_weight);
		free_bit[(bit_num >> ATOM_CACHE_BIT_ALIGN_OFFSET)] &=
			1 << (bit_num & (ATOM_CACHE_BIT_ALIGN - 1));
		nr_free++;
		*pg += nr_free;
	}
	/* TODO: not realize return unit back to atom_cache */
	return 0;
}

void *alloc_mem_node_atom_cache(struct atom_cache_desc *acd)
{
	u64 *p;
	s32 i, j, num;
	void *pfull, *pfree, *pret;

	/* highway */
	if (acd->nr_free_unit) {
		acd->nr_free_unit--; 
		p = acd->free_unit_node + 1;
		i = acd->first_unit_u64_offset;
		while (i > 0) {
			if (p[i]) {
				/* bit num from start place align u64 */
				num = i << ATOM_CACHE_BIT_ALIGN_OFFSET;
				j = __builtin_ffsll(p[i]);
				/* add last bit in u64 */
				num += j;
				j--;
				p[i] &= (~(1 << j));
			}
			i--;
		}
		pret = acd->free_unit_node + 1;
		pret += acd->first_unit_u64_offset - 1;
		/* one bit means one atom_size */
		pret += num << acd->atom_size_weight;
		p = acd->free_unit_node;
		/* how many free unit still in this node */
		num = (*p & NR_FREE_UNIT_IN_NEXT_PAGE_ADDR_MASK) - 1;
		*p = (*p & NEXT_PAGE_ADDR_MASK) + num;
		if (num == 0) {
			pfree = *(acd->free_unit_root);
			*(acd->free_unit_root) = acd->full_unit_root;
			acd->full_unit_root = acd->free_unit_root;
			acd->free_unit_root = pfree;
		}
		return pret;
	}
	/* TODO: not realize more than one node */
	return NULL;
}

static void first_memory_to_mem_node_atom_cache(struct atom_cache_desc *acd, void *addr, u64 size, s32 unit_size)
{
	s32 i, j, num, reminder, tmp;
	u64 *bit_start;

	acd->free_unit_node = addr;
	i = ATOM_CACHE_BIT_ALIGN_OFFSET;
	do {
		if (unit_size < (1 << i)) {
			break;
		}
		i++;
	} while(i < MAX_MEM_NODE_WEIGHTS);
	if (i == MAX_MEM_NODE_WEIGHTS) {
		err("unit_size %d > (1 << %d) overflow\n",
				unit_size, MAX_MEM_NODE_WEIGHTS);
		return -1;
	}
	acd->atom_size_weight = i;
	num = (1 << acd->atom_size_weight) + 1;
	num = size * 8 / (num);
	/* if space needed for free bit is aligned */
	reminder = (num / ATOM_CACHE_BIT_ALIGN) ? 1 : 0;
	num -= reminder;
	/* now num is number of unit that this memory can carry */
	acd->nr_free_unit = num;
	*((u64 *)addr) = num;

	bit_start = addr + 1;
	tmp = num / ATOM_CACHE_BIT_ALIGN;
	for (i = 0; i < tmp; i++) {
		bit_start[i] = -1ull;
	}
	bit_start[i] = 0;
	j = 0;
	while (j < num % ATOM_CACHE_BIT_ALIGN) {
		bit_start[i] |= (1 << j);
	}
	acd->first_unit_u64_offset = &bit_start[i] - addr + 1;
	return;
}

