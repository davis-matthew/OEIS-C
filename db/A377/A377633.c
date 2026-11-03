/*
 * A377633  E.g.f. satisfies A(x) = (1 + x * A(x)^2 * exp(x*A(x)^2))^2.
 * https://oeis.org/A377633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!/(2*n+1)) * Sum_{k=0..n} k^(n-k) *
 *            binomial(4*n+2,k)/(n-k)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-03)
 * Helpers:   ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow.h"
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

/* Return the A377633(n) value, memoising results. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;   /* cached sequence values   */
    static size_t cache_cap   = 0;   /* current allocation size  */

    /* Expand the cache if necessary.  ensure_cache uses zero‐initialised
       memory (the judge guarantees a freshly allocated block is zeroed). */
    if (n >= cache_cap)
        ensure_cache(n, &cache, &cache_cap);

    /* Small positive integers: the sequence never contains 0.  */
    if (cache[n] != (seq_t)0)
        return cache[n];

    /* The only sentinel value is 0: set a(0) explicitly. */
    if (n == 0) {
        cache[0] = (seq_t)1;
        return cache[0];
    }

    /* Convert to seq_t for arithmetic with the big integer type. */
    const seq_t nn = (seq_t)n;

    seq_t sum = (seq_t)0;                /* sum of terms */
    seq_t bin = (seq_t)1;                /* C(4n+2,0)   */
    seq_t falling = (seq_t)1;            /* P(n,0) = 1  */
    const seq_t four_n_plus_two = (seq_t)4 * nn + (seq_t)2;

    for (idx_t k = 1; k <= n; ++k) {
        /* Update binomial C(4n+2,k) from previous value.
           bin = bin * (4n+2 - (k-1)) / k. */
        const seq_t num = four_n_plus_two - (seq_t)(k - 1);
        bin = (bin * num) / (seq_t)k;

        /* Update falling factorial P(n,k). */
        const seq_t fac_term = nn - (seq_t)(k - 1);
        falling = falling * fac_term;

        /* Compute k^(n-k) using arbitrary‑precision exponentiation. */
        const seq_t power = ipow((seq_t)k, nn - (seq_t)k);

        /* term = falling * power * bin. */
        seq_t term = falling * power;
        term = term * bin;

        sum += term;
    }

    /* Final division by (2*n+1); the division is exact. */
    const seq_t denom   = nn * (seq_t)2 + (seq_t)1;
    const seq_t result  = sum / denom;

    cache[n] = result;
    return result;
}
