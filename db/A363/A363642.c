/*
 * A363642  Expansion of Sum_{k>0} x^k/(1 - k*x^k)^3.
 * https://oeis.org/A363642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)^{d-1} * binomial(d+1,2)
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Ensure enough space for index n (1‑based). */
    ensure_cache(n, &cache, &cache_cap);

    /* Return cached value if already computed.  All sequence values are
       positive, so a 0 value indicates “not yet computed”. */
    if (cache[n - 1] != 0)
        return cache[n - 1];

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            /* Process divisor d */
            seq_t factor  = (seq_t)(n / d);   /* n/d */
            seq_t exp     = (seq_t)(d - 1);   /* d-1 */
            seq_t pow_term = ipow(factor, exp);
            seq_t binom = ( (seq_t)(d + 1) * (seq_t)d ) / 2;
            sum += pow_term * binom;

            /* Process paired divisor if distinct */
            idx_t d2 = n / d;
            if (d2 != d)
            {
                factor  = (seq_t)(n / d2);     /* n/d2  == d */
                exp     = (seq_t)(d2 - 1);
                pow_term = ipow(factor, exp);
                binom = ( (seq_t)(d2 + 1) * (seq_t)d2 ) / 2;
                sum += pow_term * binom;
            }
        }
    }

    cache[n - 1] = sum;
    return sum;
}
