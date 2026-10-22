/*
 * A389514  Standard composition numbers of compositions whose maximal runs R
 *          all satisfy length(R) <= first(R).
 * https://oeis.org/A389514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A389514.
   a(n) = the n-th nonnegative integer k whose standard composition
   (defined via the graded reverse‑lexicographic order) satisfies the
   run condition: for every maximal run R of equal parts,
   length(R) <= first(R).
   The implementation generates successive k, tests the condition,
   and caches all qualifying indices for efficient repeated calls. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Internal cache for computed qualifying indices. */
static idx_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_k = 0;          /* next k to consider (>=0) */

/* Grow the cache to at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    idx_t *new_ptr = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
    if (new_ptr == NULL) exit(1); /* allocation failure */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Append a qualifying index to the cache. */
static void cache_add(idx_t v)
{
    if (cache_len == cache_cap) grow_cache(cache_cap ? cache_cap * 2 : 16);
    cache[cache_len++] = v;
}

/* Binary logarithm for nonzero unsigned 64‑bit integers. */
static inline int floor_log2_uint64(idx_t v)
{
    return 63 - __builtin_clzll(v);
}

/* Test whether the composition of k satisfies the maximal‑run condition. */
static bool valid_comp(idx_t k)
{
    /* Composition of 0 is the empty composition, always valid. */
    if (k == 0) return true;

    /* Determine the sum s of the composition. */
    int s = floor_log2_uint64(k) + 1;     /* s >= 1 */
    idx_t base = ((idx_t)1) << (s - 1);   /* 2^(s-1) */
    idx_t offset = k - base;              /* 0 <= offset < 2^(s-1) */

    /* Build parts from the offset bits (at most s parts). */
    int parts[64];
    int part_cnt = 0;
    int last_cut = 0;                     /* position of last cut (0 at start) */

    for (int i = 0; i < s - 1; ++i) {
        if ((offset >> i) & 1ULL) {
            int size = (i + 1) - last_cut;
            parts[part_cnt++] = size;
            last_cut = i + 1;
        }
    }
    /* Final part from last_cut to s. */
    parts[part_cnt++] = s - last_cut;

    /* Check maximal runs: length <= value of the run. */
    int cur_val = parts[0];
    int run_len = 1;
    for (int i = 1; i < part_cnt; ++i) {
        if (parts[i] == cur_val) {
            ++run_len;
        } else {
            if (run_len > cur_val) return false;
            cur_val = parts[i];
            run_len = 1;
        }
    }
    return run_len <= cur_val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* by definition a(1) is first term */

    /* Generate qualifying indices until we have enough. */
    while (cache_len < n) {
        if (next_k == 0) {
            /* k = 0 is the first qualifying index. */
            cache_add(0);
            next_k = 1;
        } else {
            if (valid_comp(next_k)) cache_add(next_k);
            ++next_k;
        }
    }

    return (seq_t)cache[n - 1];
}
