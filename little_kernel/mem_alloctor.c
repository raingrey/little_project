
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
#define GB_mask ((n << 30) - 1)
struct free_node {
	struct free_node *next;
	void *start;
};
static struct free_node *free_list_root[SUPPORTED_FREE_LIST_TYPE_NR] = NULL;

static struct free_node *decrease_free_node(enum free_list_type flt)
{
	struct free_node *node;

	if (free_list_root[flt] == NULL) {
		return NULL;
	}
	node = free_list_root[flt];
	free_list_root[flt] = free_list_root[flt]->next;
}

static inline void increase_free_node(struct free_node *fn, enum free_list_type flt)
{
	struct free_node *node;

	fn->next = free_list_root[flt];
	free_list_root[flt] = fn;
}

static s32 release_memory(void *start, u64 size)
{
	struct free_node * fn;

	if (size < KB(4)) {
		err("release invalid size 0x%llx\n", size);
		return -1;
	}
	fn = alloc_unit(sizeof(struct free_node));
	if (fn == NULL) {
		
	}
}

struct atom_cache_unit {
	struct atom_cache_unit *next_free_unit;
};

struct atom_cache_desc {
	u64 atom_size;
	struct atom_cache_unit *free_unit_root;
}

struct free_list_atom_cache {

};
struct mem_chunk {
	void *addr;
	u64 size;
};
s32 setup_free_list(struct mem_chunk *mc)
{
	s32 i = 0, sum = 0;

	while (mc[i] != NULL) {

		/* calulate how many atom_cache_unit is need */
		sum += mc[i].size & (~(u64)GB_mask);
		sum += mc[i].size & (~(u64)GB_mask);


		/* use mem save atom_cache_unit from start or scattered memory */

	}
	free_list_root
}


static void *create_unit_cache_on_mem_node(
		struct atom_cache_desc **unit_cache_root, void *addr,
	       	enum free_list_type flt, u64 unit_size)
{
	struct atom_cache_desc *acd = addr;


}
