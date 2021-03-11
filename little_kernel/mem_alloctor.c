
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
static struct free_node *free_list_root
			[MAX_MEM_NODE_WEIGHTS - MAX_MEM_NODE_WEIGHTS] = NULL;

static inline struct free_node *decrease_free_node(s32 weight)
{
	struct free_node *node;

	node = free_list_root[weight];
	free_list_root[weight] = free_list_root[weight]->next;
	return node;
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

struct free_node *alloc_mem_node(s32 weight)
{
	struct free_node *fn;
	void *p;

	if (weight > MAX_MEM_NODE_WEIGHTS) {
		err("weight overflow %d > %d\n", weight, MAX_MEM_NODE_WEIGHTS);
		return NULL;
	}
	if (free_list_root[weight]) {
		return decrease_free_node(weight);
	}
	fn = alloc_mem_node(weight + 1);
	if (fn == NULL) {
		return NULL;
	}
	p = fn->start;
	fn->start = p + (1 << weight);
	increase_free_node(fn, weight);
	fn = alloc_mem_node_atom_cache();
	fn->start = p;
	return fn;
}

struct atom_cache_unit {
	struct atom_cache_unit *next_free_unit;
	void *addr;
};

static struct atom_cache_desc mem_node_atom_cache = {
	.nr_free_unit = 0;
	.first_unit_u64_offset = 0;
	.atom_size_weight = 4; //unit size 16 byte
	.used_mem_node_weight = 12; /* 4KB */
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




s32 setup_free_list(void *addr, u64 size)
{
	s32 i = 0, sum = 0, weight;
	void *cur, *end;
	struct free_node *fn, *fn_next;

	cur = addr;
	end = addr + size;
	weight = MAX_MEM_NODE_WEIGHTS;
again:
	while (weight < MIN_MEM_NODE_WEIGHTS) {
		if (((1 << weight) - 1) & cur) {
			weight--;
		} else {
			break;
		}
	}
	if (weight < MIN_MEM_NODE_WEIGHTS) {
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
		weight = MAX_MEM_NODE_WEIGHTS;
		goto again;
	} else if (cur + (1 << weight) > end) {
		weight--;
		goto again;
	}
	return 0;
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
	ret = setup_free_list(addr + KB(4), size - KB(4));
	if (ret < 0) {
		return ret;
	}

}
