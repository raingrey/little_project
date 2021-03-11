#ifndef __MEM_H_
#define __MEM_H_
#define dbg(fmt, ...) printf("D: %s@%d: "fmt, __func__, __LINE__, ##__VA_ARGS__);
#define err(fmt, ...) printf("E: %s@%d: "fmt, __func__, __LINE__, ##__VA_ARGS__);

#define ATOM_CACHE_BIT_ALIGN_OFFSET 6
#define ATOM_CACHE_BIT_ALIGN (1 << 6) //64

#define MAX_MEM_NODE_WEIGHTS 30/* 1GB */
#define MIN_MEM_NODE_WEIGHTS 12/* 4KB */
struct atom_cache_desc {
	u32 nr_free_unit;
	/* for all mem_node used by this cache, this is same */
	u16 first_unit_u64_offset;
	/* TODO: please check if target size power of 2,
	 * use atom_size_weight if it is,
	 * and atom_size_u64 for other case
	 */
	union {
		u8 atom_size_weight;
		/* TODO: please grep atom_size_weight
		 * and replace (1 << atom_size_weight) with
		 * (atom_size_u64 * ATOM_CACHE_BIT_ALIGN) */
		u8 atom_size_u64;
	}
	u8 used_mem_node_weight;
	void *full_unit_node;
	void *free_unit_node;
};

#endif
