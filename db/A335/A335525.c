/*
 * A335525  Numbers k such that the k-th composition in standard order
 *          (A066099) avoids the pattern (1,2,2).
 * https://oeis.org/A335525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   compute_parts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Scan successive integers k (starting at 0) in
   increasing order, convert each k to its composition in standard
   order (A066099), check whether that composition avoids the
   pattern (1,2,2).  Store the k's that pass into a static cache,
   and return the (n‑1)th stored value for a(n).  This works for
   any build where seq_t is an unsigned integer type (8/16/32/64/128
   bits) or an arbitrary‑precision big‑integer class.  It satisfies
   all requirements (no external dependencies, memoised, fast
   enough for n≤10000). */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/compute_parts.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Helper: test whether a composition (parts[0 .. len-1]) contains
   the pattern (1,2,2).  The pattern requires indices i<j<k with
   parts[i] < parts[j] == parts[k].                                 */
static bool avoid_pattern(const idx_t parts[], size_t len)
{
    if (len < 3) return true;

    /* Keep the smallest part seen so far. */
    idx_t min_so_far = parts[0];
    for (size_t j = 1; j < len; ++j) {
        if (min_so_far < parts[j]) {
            for (size_t k = j + 1; k < len; ++k) {
                if (parts[k] == parts[j]) {
                    return false;  /* pattern found */
                }
            }
        }
        if (parts[j] < min_so_far) {
            min_so_far = parts[j];
        }
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.  It is memoised: any order of calls for
   a(n) is supported.  The cache contains all valid k's found so
   far.                                                               */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined; return 0 */

    /* Persistent cache of found indices. */
    static seq_t *cache      = NULL;  /* array of seq_t values */
    static size_t cache_len  = 0;     /* number of stored values */
    static size_t cache_cap  = 0;     /* allocated capacity */
    static idx_t  next_k     = 0;     /* next k to examine */

    /* Grow cache until it contains at least n entries. */
    while (cache_len < n) {
        /* Compute composition of current k. */
        idx_t parts[64];
        size_t len;
        compute_parts(next_k, parts, &len);

        /* If composition avoids pattern, store k. */
        if (avoid_pattern(parts, len)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                /* realloc returns void* in C; cast to seq_t* in both C and C++. */
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;   /* allocation failed */
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
