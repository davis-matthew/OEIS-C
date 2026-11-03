/*
 * A285008  Numerator of (3/4)^n * binomial(2*n,n).
 * https://oeis.org/A285008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(0) = 1 For n ≥ 1: a(n) = 3*(2*n-1)*a(n‑1)/odd_divisor(n)
 *            (odd_divisor(n) = largest odd divisor of n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 904 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is the numerator of (3/4)^n * binomial(2n,n).  The
   recurrence above follows from the identity
   a(n) = 3^n * binomial(2n,n) / 2^{popcount(n)}  and the fact that
   popcount(n) = number of factors of 2 in binomial(2n,n).  Dividing
   by the odd divisor first keeps intermediate values small enough to
   avoid overflow on 64‑bit builds, while still working for arbitrary‑
   precision integers.  A static cache is used so repeated calls in
   arbitrary order are efficient. */

#include <stddef.h>
#include <stdbool.h>
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

static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* 0 is always 1 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Allocate or extend cache to at least n+1 entries */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise the first term if not already set */
    if (cache[0] == (seq_t)0)
        cache[0] = (seq_t)1;

    /* Compute terms up to n, filling any missing entries */
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        if (cache[i] != (seq_t)0)            /* already computed */
            continue;

        /* Previous term */
        seq_t prev = cache[i - (idx_t)1];

        /* largest odd divisor of i */
        idx_t odd = i;
        while ((odd & (idx_t)1) == (idx_t)0)
            odd >>= (idx_t)1;

        /* Integer division is exact because odd divides prev */
        seq_t reduced = prev / (seq_t)odd;

        /* Multiply by 3*(2*i-1) */
        seq_t mult = (seq_t)3 * ((seq_t)2 * i - (seq_t)1);
        cache[i] = reduced * mult;
    }

    return cache[n];
}
