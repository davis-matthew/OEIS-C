/*
 * A362332  For n > 1, if n appears in the sequence then a(n) = lastindex(n),
 *          where lastindex(n) is the index of the last appearance of n.
 *          Otherwise a(n+1) = a(n)/(n+1) if (n+1)|a(n), otherwise a(n)*(n+1),
 *          a(1) = 1 and a(2) = 1*2.
 * https://oeis.org/A362332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 32, u64 n <= 46, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS definition:  a(1)=1, a(2)=2.
   For n>1,
      if n appears in the sequence before, then a(n) = last_index(n);
      otherwise a(n) = a(n‑1)/n if a(n‑1) divisible by n, else a(n‑1)*n.   */

/* Implementation with static memoisation and a simple map for last indices. */
#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* cache[1..] holds a(n) */
static size_t cache_cap = 0;         /* allocated size of cache (≥ current max index+1) */
static idx_t  computed_up_to = 0;    /* highest index already computed */

struct MapEntry {
    seq_t value;
    idx_t idx;            /* last index where value appeared */
};
static struct MapEntry *map = NULL;
static size_t map_len = 0, map_cap = 0;

/* Update the map for a key; replace index if key already present. */
static void map_update(seq_t key, idx_t idx_val) {
    /* linear search for existing key */
    for (size_t j = 0; j < map_len; ++j) {
        if (map[j].value == key) {
            map[j].idx = idx_val;
            return;
        }
    }
    /* reallocate if necessary */
    if (map_len == map_cap) {
        size_t new_cap = map_cap ? map_cap * 2 : 4;
        struct MapEntry *tmp = (struct MapEntry *)realloc(map, new_cap * sizeof(struct MapEntry));
        if (!tmp) exit(1);   /* abort on allocation failure */
        map = tmp;
        map_cap = new_cap;
    }
    map[map_len].value = key;
    map[map_len].idx   = idx_val;
    ++map_len;
}

/* Return last index where a particular value appeared; 0 if never. */
static idx_t map_find(seq_t key) {
    for (size_t j = 0; j < map_len; ++j) {
        if (map[j].value == key) return map[j].idx;
    }
    return 0;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n < 1 */

    /* Ensure cache large enough */
    if (n + 1 > (idx_t)cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to requested index, if not already done */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
            map_update((seq_t)1, 1);
        } else if (i == 2) {
            cache[2] = (seq_t)2;
            map_update((seq_t)2, 2);
        } else {
            idx_t last_occ = map_find((seq_t)i);   /* check if i has appeared before */
            seq_t val;
            if (last_occ != 0) {
                val = (seq_t)last_occ;
            } else {
                seq_t prev = cache[i - 1];
                if (prev % (seq_t)i == 0)
                    val = prev / (seq_t)i;
                else
                    val = prev * (seq_t)i;
            }
            cache[i] = val;
            map_update(val, i);
        }
    }

    computed_up_to = n;
    return cache[n];
}
