
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

static void *create_unit_cache_from_memory_node(unit_cache_root, void *start, u64 mem_size, u64 unit_size)
{
	if (mem_size < unit_size) {
		err("cache unit %lld > mem_size %lld\n", unit_size, mem_size);
		return -1;
	}
	

}
