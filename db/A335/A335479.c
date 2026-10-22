/*
 * A335479  Numbers k such that the k-th composition in standard order
 *          (A066099) matches the pattern (1,2,3).
 * https://oeis.org/A335479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   comp_has_inc_triplet
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th integer k such that the composition of k in standard
   order (A066099) contains an increasing subsequence of length 3
   (i.e., matches the pattern (1,2,3)). Computed by brute‑force search. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/comp_has_inc_triplet.h"

static seq_t *cache = NULL;       /* cached k‑values */
static size_t cache_len = 0;      /* number of cached terms */
static size_t cache_cap = 0;      /* capacity of cache array */
static idx_t next_k = 1;          /* next k to examine */

/* Append a value to the cache, expanding if necessary. */
static void cache_append(seq_t val) {
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n < 1, return 0 */
    /* Ensure cache contains at least n terms. */
    while (cache_len < n) {
        if (comp_has_inc_triplet(next_k)) {
            /* Store the (1‑based) k value. */
            cache_append((seq_t)next_k);
        }
        ++next_k;
    }
    return cache[n - 1];
}
