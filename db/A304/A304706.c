/*
 * A304706  Number of partitions (d1,d2,...,dm) of n such that d1/1 > d2/2 >
 *          ... > dm/m and 0 < d1 <= d2 <= ... <= dm.
 * https://oeis.org/A304706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions (d1,d2,…,dm) of n such that d1/1 >
 *            d2/2 > … > dm/m and 0 < d1 ≤ d2 ≤ … ≤ dm. This routine
 *            implements a memoised depth‑first search of all admissible
 *            partitions
 * Range:     u8 n <= 53, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    idx_t rem;   /* remaining sum to partition */
    idx_t idx;   /* number of parts already chosen (next part will be (idx+1)) */
    idx_t last;  /* value of the last chosen part; 0 when no part yet */
} StateKey;

typedef struct {
    StateKey key;
    seq_t    val;
    int      used;  /* 0 = empty slot, 1 = occupied */
} HTEntry;

static HTEntry *ht   = NULL; /* hash array */
static size_t  ht_cap = 0;   /* capacity (power of 2) */
static size_t  ht_used = 0;  /* number of used slots */

/* ----- hash utilities ---------------------------------------------------- */
static inline uint64_t hash_uint64(uint64_t v)
{
    v ^= v >> 33;
    v *= 0xff51afd7ed558ccdULL;
    v ^= v >> 33;
    v *= 0xc4ceb9fe1a85ec53ULL;
    v ^= v >> 33;
    return v;
}

static inline size_t key_hash(const StateKey *k)
{
    return (size_t)(hash_uint64((uint64_t)k->rem) ^
                    (hash_uint64((uint64_t)k->idx) << 1) ^
                    (hash_uint64((uint64_t)k->last) << 2));
}

/* ----- table management --------------------------------------------------- */
static void ht_resize(void)
{
    size_t new_cap = ht_cap ? ht_cap << 1 : 1 << 12;
    HTEntry *new_tbl = (HTEntry *)calloc(new_cap, sizeof(HTEntry));
    if (!new_tbl) abort();                /* allocation failure */

    for (size_t i = 0; i < ht_cap; ++i) {
        if (ht[i].used) {
            size_t h = key_hash(&ht[i].key) & (new_cap - 1);
            while (new_tbl[h].used) h = (h + 1) & (new_cap - 1);
            new_tbl[h] = ht[i];
        }
    }
    free(ht);
    ht     = new_tbl;
    ht_cap = new_cap;
}

static seq_t ht_lookup(const StateKey *k)
{
    if (!ht) return 0;                  /* table empty → not found */
    size_t h = key_hash(k) & (ht_cap - 1);
    while (ht[h].used) {
        if (ht[h].key.rem == k->rem &&
            ht[h].key.idx == k->idx &&
            ht[h].key.last == k->last)
            return ht[h].val;
        h = (h + 1) & (ht_cap - 1);
    }
    return 0;   /* not found; 0 is a valid value but the slot is unused */
}

static void ht_insert(const StateKey *k, seq_t val)
{
    if (!ht) ht_resize();               /* first allocation */
    size_t h = key_hash(k) & (ht_cap - 1);
    while (ht[h].used) {
        if (ht[h].key.rem == k->rem &&
            ht[h].key.idx == k->idx &&
            ht[h].key.last == k->last) {
            ht[h].val = val;            /* overwrite existing entry */
            return;
        }
        h = (h + 1) & (ht_cap - 1);
    }
    ht[h].used = 1;
    ht[h].key  = *k;
    ht[h].val  = val;
    ++ht_used;

    /* resize when load factor exceeds 0.75 */
    if (ht_used * 4 > ht_cap * 3) ht_resize();
}

/* ----- recursive counting ----------------------------------------------- */
static seq_t dfs(idx_t rem, idx_t idx, idx_t last)
{
    if (rem == 0) return (seq_t)1;   /* full partition found */

    StateKey k = { rem, idx, last };
    seq_t cached = ht_lookup(&k);
    if (cached) return cached;       /* cached result (including zero) */

    seq_t total = 0;
    if (idx == 0) {                  /* first part: no ratio restriction */
        for (idx_t j = 1; j <= rem; ++j)
            total += dfs(rem - j, 1, j);
    } else {
        /* ratio constraint: j <= ((idx+1)*last - 1) / idx */
        idx_t maxj = ((idx + 1) * last - 1) / idx;
        if (maxj > rem) maxj = rem;
        for (idx_t j = last; j <= maxj; ++j)
            total += dfs(rem - j, idx + 1, j);
    }

    ht_insert(&k, total);
    return total;
}

/* ----- public interface --------------------------------------------------- */
seq_t a(idx_t n)
{
    return dfs(n, 0, 0);
}
