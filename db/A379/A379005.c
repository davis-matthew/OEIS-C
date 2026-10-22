/*
 * A379005  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => v_2(i) = v_2(j), v_3(i) = v_3(j) and v_5(i) = v_5(j), for all
 *          i, j, where v_2 (A007814), v_3 (A007949) and v_5 (A112765) give
 *          the 2-, 3- and 5-adic valuations of n respectively.
 * https://oeis.org/A379005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the lexicographically earliest infinite sequence such
 *            that a(i) = a(j) implies v_2(i) = v_2(j), v_3(i) = v_3(j) and
 *            v_5(i) = v_5(j)
 * Range:     u8 n <= 9836, u16 n <= 9836, u32 n <= 9836, u64 n <= 9836,
 *            u128 n <= 9836, big any n
 * Helpers:   hash32, valuations
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/hash32.h"
#include "../utils/valuations.h"

typedef uint64_t idx_t;   /* index type */

static struct HashEntry {
    uint32_t key;
    seq_t val;
    bool used;
} *hash_table = NULL;
static size_t hash_cap = 0;   /* must be power of two */
static size_t hash_used = 0;

static seq_t *cache = NULL;   /* cache of computed values */
static size_t cache_cap = 0;  /* allocated size of cache */
static seq_t next_val = 1;    /* next unused value for a new triple */

static void hash_init(void)
{
    hash_cap = 64;
    hash_used = 0;
    hash_table = (struct HashEntry*)calloc(hash_cap, sizeof(struct HashEntry));
}

static void hash_rehash(void)
{
    size_t new_cap = hash_cap * 2;
    struct HashEntry *new_tab = (struct HashEntry*)calloc(new_cap, sizeof(struct HashEntry));
    for (size_t i = 0; i < hash_cap; ++i) {
        if (hash_table[i].used) {
            uint32_t k = hash_table[i].key;
            seq_t v = hash_table[i].val;
            size_t idx = hash32(k) & (new_cap - 1);
            while (new_tab[idx].used) idx = (idx + 1) & (new_cap - 1);
            new_tab[idx].used = true;
            new_tab[idx].key   = k;
            new_tab[idx].val   = v;
        }
    }
    free(hash_table);
    hash_table = new_tab;
    hash_cap = new_cap;
}

static bool hash_find(uint32_t key, seq_t *out_val)
{
    if (!hash_cap) return false;
    size_t idx = hash32(key) & (hash_cap - 1);
    for (;;) {
        if (!hash_table[idx].used) return false;
        if (hash_table[idx].key == key) {
            *out_val = hash_table[idx].val;
            return true;
        }
        idx = (idx + 1) & (hash_cap - 1);
    }
}

static void hash_insert(uint32_t key, seq_t val)
{
    if (!hash_cap) hash_init();
    if ((hash_used + 1) * 10 >= hash_cap * 7) hash_rehash();   /* load factor > 0.7 */
    size_t idx = hash32(key) & (hash_cap - 1);
    while (hash_table[idx].used) idx = (idx + 1) & (hash_cap - 1);
    hash_table[idx].used = true;
    hash_table[idx].key  = key;
    hash_table[idx].val  = val;
    ++hash_used;
}

/* ----------------- ensure cache up to n ----------------- */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap) return;
    size_t old_cap = cache_cap;
    cache_cap = n;
    cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    for (size_t i = old_cap; i < cache_cap; ++i) {
        idx_t idx = i + 1;                          /* indices start at 1 */
        uint8_t v2, v3, v5;
        valuations(idx, &v2, &v3, &v5);
        uint32_t key = ((uint32_t)v2 << 16) | ((uint32_t)v3 << 8) | v5;
        seq_t val;
        if (hash_find(key, &val))
            cache[i] = val;          /* seen before */
        else {
            val = next_val++;
            hash_insert(key, val);
            cache[i] = val;          /* new triple */
        }
    }
}

/* ----------------- public function ----------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, behaviour for n<1 is irrelevant */
    ensure_cache(n);
    return cache[n - 1];
}
