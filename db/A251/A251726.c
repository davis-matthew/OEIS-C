/*
 * A251726  Numbers n > 1 for which gpf(n) < lpf(n)^2, where lpf and gpf
 *          (least and greatest prime factor of n) are given by A020639(n) and
 *          A006530(n).
 * https://oeis.org/A251726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf, lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A251726: Numbers n > 1 with gpf(n) < lpf(n)^2.
   This implementation builds the sequence by testing successive integers
   and caching the qualifying ones.  It relies on the helper functions
   lpf() and gpf() from ../utils/lpf.h and ../utils/gpf.h.    */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/lpf.h"
#include "../utils/gpf.h"

/* Cached sequence values (index 1 → first term). */
static seq_t *seq_cache = NULL;    /* array of values */
static size_t cache_len = 0;       /* number of stored terms */
static size_t cache_cap = 0;       /* allocated capacity */
static uint64_t next_candidate = 2;/* next integer to test */

/* Grow the cache array to hold at least `need` elements. */
static void grow_cache(size_t need)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 1024;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);   /* fallback on allocation failure */
    seq_cache = tmp;
    cache_cap  = new_cap;
}

/* Return true if m satisfies gpf(m) < lpf(m)^2. */
static inline bool qualifies(uint64_t m)
{
    if (m <= 1) return false;
    uint64_t l = lpf((idx_t)m);   /* least prime factor */
    uint64_t g = gpf(m);          /* greatest prime factor */

    /* Avoid overflow: if l*l would overflow 64‑bit, the inequality
       is automatically true because g <= m <= 2^64-1. */
    if (l > UINT64_MAX / l) return true;
    return g < l * l;
}

/* Return the nth term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 */

    /* Ensure the cache is initialized. */
    if (seq_cache == NULL) {
        cache_cap = 0;
        cache_len = 0;
        next_candidate = 2;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        if (qualifies(next_candidate)) {
            if (cache_len >= cache_cap) grow_cache(cache_len + 1);
            seq_cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return seq_cache[(size_t)n - 1];
}
