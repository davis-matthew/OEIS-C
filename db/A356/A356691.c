/*
 * A356691  a(n) = n! * Sum_{k=0..n} k^(2*k)/k!.
 * https://oeis.org/A356691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * a(n-1) + n^(2*n), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-03)
 * Helpers:   ipow_u64_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_u64_idx.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Static cache: a[0]..a[size-1] */
    static seq_t *cache   = NULL; /* array of computed values */
    static size_t cap     = 0;     /* allocated capacity */
    static size_t size    = 0;     /* next index to compute */

    /* Initialise cache with a(0) if not yet done */
    if (size == 0) {
        cap   = 1;
        cache = mem_new(seq_t, cap);
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        size    = 1;
    }

    /* Grow cache if necessary */
    if (n + 1 > (idx_t)cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        cache = mem_resize(seq_t, cache, new_cap);
        cap   = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = size; i <= n; ++i) {
        seq_t term;
        if (i == 0) {
            term = (seq_t)1;
        } else {
            seq_t pow_term = ipow_u64_idx((uint64_t)i, 2 * i);
            term = (seq_t)i * cache[(size_t)(i - 1)] + pow_term;
        }
        cache[(size_t)i] = term;
    }

    size = (size_t)(n + 1);
    return cache[(size_t)n];
}
