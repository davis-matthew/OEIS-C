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
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th integer k such that the composition of k in standard
   order (A066099) contains an increasing subsequence of length 3
   (i.e., matches the pattern (1,2,3)). Computed by brute‑force search. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Determine whether the k‑th composition in standard order contains an
   increasing subsequence of length 3. */
static bool comp_has_inc_triplet(idx_t k) {
    /* Compute positions of set bits (1‑based) in the binary expansion
       of k, read from least significant to most significant. */
    int diffs[64];    /* differences between successive positions (including 0) */
    int dlen = 0;
    int prev_pos = 0;
    idx_t temp = k;
    int pos = 1;
    while (temp) {
        if (temp & 1ULL) {
            diffs[dlen++] = pos - prev_pos;
            prev_pos = pos;
        }
        pos++;
        temp >>= 1;
    }
    /* Reverse diffs to obtain the composition parts. */
    int comp[64];
    for (int i = 0; i < dlen; ++i) {
        comp[i] = diffs[dlen - 1 - i];
    }
    /* Brute‑force check for an increasing subsequence of length 3. */
    for (int i = 0; i < dlen; ++i) {
        for (int j = i + 1; j < dlen; ++j) {
            if (comp[i] < comp[j]) {
                for (int k2 = j + 1; k2 < dlen; ++k2) {
                    if (comp[j] < comp[k2]) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

static seq_t *cache = NULL;       /* cached k‑values */
static size_t cache_len = 0;      /* number of cached terms */
static size_t cache_cap = 0;      /* capacity of cache array */
static idx_t next_k = 1;          /* next k to examine */

/* Append a value to the cache, expanding if necessary. */
static void cache_append(seq_t val) {
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
        seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
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
