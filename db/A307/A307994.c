/*
 * A307994  G.f. A(x) satisfies: A(x) = x + x^2 + x^3 + x^4 * (1 + A(x) +
 *          A(x^2) + A(x^3) + ...).
 * https://oeis.org/A307994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 74 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A307994
   Recurrence:  a(1) = a(2) = a(3) = a(4) = 1,
                a(n+4) = Σ_{d|n} a(d)   for n ≥ 1        */

#include <stddef.h>          /* size_t */
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

static seq_t *cache = NULL;       /* a(1) … a(current_max) */
static size_t cache_cap = 0;      /* allocated capacity of cache */
static idx_t cache_size = 0;      /* largest n for which values are known */

seq_t a(idx_t n)
{
    /* Handle the initial four terms directly. */
    if (n <= 4)
        return 1;

    /* Place a(1)…a(4) into the cache if we have not yet done so. */
    if (cache == NULL) {
        /* Reserve space for at least the first 4 elements. */
        ensure_cache(4, &cache, &cache_cap);
        cache[0] = cache[1] = cache[2] = cache[3] = 1;
        cache_size = 4;
    }

    /* Expand the cache to hold the requested index. */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        idx_t m   = i - 4;          /* argument for divisor sum   */
        seq_t sum = 0;

        /* Sum a(d) for all divisors d of m. */
        for (idx_t d = 1; d <= m / d; ++d) {
            if (m % d == 0) {
                /* a(d) */
                sum += cache[d - 1];
                /* a(m/d) if distinct from d */
                if (d != m / d)
                    sum += cache[(m / d) - 1];
            }
        }
        cache[i - 1] = sum;
    }

    cache_size = n;   /* mark all values up to n as computed */
    return cache[n - 1];
}
