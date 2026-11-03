/*
 * A343511  a(n) = 1 + Sum_{d|n, d < n} a(d)^2.
 * https://oeis.org/A343511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{d|n, d<n} a(d)^2 Recurrence: a(1) = 1, a(n) = 1 +
 *            Σ_{d|n, d<n} a(d)^2 (n ≥ 2)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Validated: standard, pass, 81 terms through n = 2889 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                     /* size_t */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n)
{
    /* static cache: a[0] is unused, a[1] ... a[max_computed] are valid */
    static seq_t *cache          = NULL;  /* array of seq_t */
    static size_t cache_cap      = 0;     /* allocated length (≥ max_computed+1) */
    static idx_t  max_computed   = 0;     /* highest index already computed */

    if (n == 0)                         /* safety guard – not used in OEIS offset */
        return (seq_t)0;

    /* (Re)allocate if the cache is too small for index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;          /* need indices 0 … n */
        seq_t *new_cache = seq_alloc_new_array(new_cap);

        /* initialise all elements to zero */
        for (size_t i = 0; i < new_cap; ++i)
            new_cache[i] = (seq_t)0;

        /* copy old data (if any) into the new array */
        if (cache != NULL) {
            for (size_t i = 0; i < cache_cap; ++i)
                new_cache[i] = cache[i];
            free_seq_array(cache);
        }
        cache      = new_cache;
        cache_cap  = new_cap;
    }

    /* Compute missing entries in increasing order */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                idx_t d2 = i / d;
                if (d != i)                 /* proper divisor */
                    sum += cache[d] * cache[d];
                if (d2 != d && d2 != i)     /* distinct second divisor */
                    sum += cache[d2] * cache[d2];
            }
        }
        cache[i] = sum + (seq_t)1;               /* a(i) = 1 + sum */
    }
    max_computed = n;

    return cache[n];
}
