/*
 * A336950  E.g.f.: 1 / (1 - x * exp(2*x)).
 * https://oeis.org/A336950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * k * 2^(k-1) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 370 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
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

/* Static cache for memoisation */
static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;

seq_t a(idx_t n)
{
    /* Ensure the cache can hold all terms up to n */
    ensure_cache(n, &a_cache, &a_cache_cap);

    /* Initialise a(0) if it has not been computed yet */
    if (a_cache[0] == (seq_t)0)
        a_cache[0] = (seq_t)1;

    /* If the requested term is already cached, return it */
    if (a_cache[n] != (seq_t)0)
        return a_cache[n];

    /* Compute all missing terms up to n */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (a_cache[i] != (seq_t)0)
            continue;                /* already computed */

        seq_t sum = 0;
        seq_t binom = 1;              /* C(i,0) = 1 */

        for (idx_t k = 1; k <= i; ++k)
        {
            /* Compute C(i,k) using a trick to avoid intermediate overflow.
               Reduce numerator and denominator by their gcd with binom. */
            seq_t num = (seq_t)(i - k + 1);   /* (i - k + 1) */
            seq_t den = (seq_t)k;             /* k */
            seq_t g = gcd(num, den);
            num /= g;  den /= g;
            seq_t g2 = gcd(binom, den);
            binom /= g2;  den /= g2;
            /* At this point den should be 1 */
            binom *= num;                       /* new binom = C(i,k) */

            /* term = C(i,k) * k * 2^(k-1) * a(i-k) */
            seq_t term = binom;
            term *= k;                          /* multiply by k */

            /* multiply by 2^(k-1) by repeated shifting of 1 */
            for (idx_t p = 1; p < k; ++p)
                term <<= 1;

            term *= a_cache[i - k];              /* final multiplication */

            sum += term;
        }

        a_cache[i] = sum;
    }

    return a_cache[n];
}
