
/* TODO: not realized interface currently */
#define kmalloc malloc
#define err printf

/* supported free list types:
 * 1	GB
 * 16	MB
 * 256	KB
 * 4	KB
 */
enum free_list_type {
	BY_4KB,
	BY_256KB,
	BY_16MB,
	BY_GB,
	SUPPORTED_FREE_LIST_TYPE_NR 
}
#define KB(n) (n << 10)
#define GB(n) (n << 30)
#define GB_mask ((1 << 31) - 1)
struct free_node {
	struct free_node *next;
	void *start;
};
#define MAX_MEM_MODE_WEIGHTS 30/* 1GB */
#define MIN_MEM_MODE_WEIGHTS 12/* 4KB */
static struct free_node *free_list_root
			[MAX_MEM_MODE_WEIGHTS - MAX_MEM_MODE_WEIGHTS] = NULL;

static struct free_node *decrease_free_node(s32 weight)
{
	struct free_node *node;

	if (free_list_root[weight] == NULL) {
		return NULL;
	}
	node = free_list_root[weight];
	free_list_root[weight] = free_list_root[weight]->next;
}

static inline void increase_free_node(struct free_node *fn, s32 weight)
{
	struct free_node *node;

	fn->next = free_list_root[weight];
	free_list_root[weight] = fn;
}

static s32 release_memory(void *start, u64 size)
{
	struct free_node * fn;

	if (size < KB(4)) {
		err("release invalid size 0x%llx\n", size);
		return -1;
	}
	fn = alloc_free_node_atom_cache();
	if (fn == NULL) {
		
	}
}

struct atom_cache_unit {
	struct atom_cache_unit *next_free_unit;
	void *addr;
};
#define ATOM_CACHE_BIT_ALIGN_OFFSET 6
#define ATOM_CACHE_BIT_ALIGN (1 << 6) //64
struct atom_cache_desc {
	u32 nr_free_unit;
	u16 first_unit_u64_offset;
	u16 atom_size_u64_offset;
	void *full_unit_node;
	void *free_unit_node;
}

static struct atom_cache_desc mem_node_atom_cache = {
	.nr_free_unit = 0;
	.first_unit_u64_offset = 0;
	.atom_size_u64_offset = 1;//unit size 16, (1 << 1)u64
	.full_unit_node = NULL;
	.free_unit_node = NULL;
};



struct free_list_atom_cache {

};
struct mem_chunk {
	void *addr;
	u64 size;
};


static void *create_unit_cache_on_mem_node(
		struct atom_cache_desc **unit_cache_root, void *addr,
	       	s32 weight, u64 unit_size)
{
	struct atom_cache_desc *acd = addr;


}

#define NEXT_PAGE_ADDR_MASK (~((1 << 9) - 1))
#define NR_FREE_UNIT_IN_NEXT_PAGE_ADDR_MASK ((1 << 9) - 1)
static void *alloc_mem_node_atom_cache(void)
{
	struct atom_cache_desc *acd = &mem_node_atom_cache;
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
				num = i << ATOM_CACHE_BIT_ALIGN_OFFSET;
				j = __builtin_ffsll(p[i]);
				num += j;
				j--;
				p[i] &= (~(1 << j));
			}
		}
		pret = acd->free_unit_node + 1 + acd->first_unit_u64_offset - 1
			+ num << acd->atom_size_u64_offset;
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
}

s32 free_mem_node_atom_cache(void)
{
	/* TODO: not realize return unit back to atom_cache */
	return 0;
}

static void first_memory_to_mem_node_atom_cache(void *addr, u64 size)
{
	s32 i, j, num, reminder, tmp;
	u64 *bit_start;
	struct atom_cache_desc *acd = &mem_node_atom_cache;

	mem_node_atom_cache.free_unit_node = addr;
	num = acd.atom_size_u64_offset * ATOM_CACHE_BIT_ALIGN + 1;
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

s32 setup_free_list(void *addr, u64 size)
{
	s32 i = 0, sum = 0, weight;
	void *cur, *end;
	struct free_node *fn, *fn_next;

	cur = addr;
	end = addr + size;
	weight = MAX_MEM_MODE_WEIGHTS;
again:
	while (weight < MIN_MEM_MODE_WEIGHTS) {
		if (((1 << weight) - 1) & cur) {
			weight--;
		} else {
			break;
		}
	}
	if (weight < MIN_MEM_MODE_WEIGHTS) {
		err("Invalid addr 0x%llx\n", addr);
		return -1;
	}
	if (cur + (1 << weight) < end) {
		/* as new free_node inserting, this alloc will never fail */
		fn = alloc_mem_node_atom_cache();
		fn->next = free_list_root[weight]->next;
		free_list_root[weight]->fn;
		sum++;
		cur += (1 << weight);
		weight = MAX_MEM_MODE_WEIGHTS;
		goto again;
	} else if (cur + (1 << weight) > end) {
		weight--;
		goto again;
	}
}

/* allowed minimum memory node is 4KB */
#define KERNEL_MIN_BOOT_MEMORY (MB(1))
#define MEM_NODE_SIZE_MIN_MASK ((KB(4) << 1) - 1)
s32 start_memory(void *addr, u64 size)
{
	s32 ret;

	if ((addr & MEM_NODE_SIZE_MIN_MASK) || size < KERNEL_MIN_BOOT_MEMORY) {
		err("addr 0x%llx not aligned or mem size 0x%llx too small\n",
				addr, size);
		return -1;
	}
	first_memory_to_mem_node_atom_cache(addr, KB(4));

}
