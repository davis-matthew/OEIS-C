/*
 * A355117  a(1) = 1; a(n+1) = Sum_{d|n} 4^(n/d - 1) * a(d).
 * https://oeis.org/A355117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A355117:
   a(1) = 1
   a(n+1) = Sum_{d|n} 4^(n/d-1) * a(d)   (for n ≥ 1) */
#include <stddef.h>             /* size_t */
#include <stdlib.h>             /* malloc, realloc, abort */

#include "../utils/pow_uint.h"  /* seq_t pow_uint(uint64_t base, idx_t exp) */

static seq_t *cache = NULL;      /* cache[0] unused; cache[1] = a(1) */
static idx_t cache_len = 0;      /* size of allocated cache (>= 1 if set) */

/* Compute a(n) for any n ≥ 1 */
seq_t a(idx_t n)
{
    if (n < 1)                     /* undefined for n < 1, return 0 */
        return 0;

    /* Grow the cache to contain indices up to n */
    if (cache_len <= n) {
        size_t needed = (size_t)n + 1;          /* indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache,
                                      needed * sizeof(seq_t));
        if (tmp == NULL)
            abort();                            /* allocation failed */
        cache = tmp;
        idx_t old_len = cache_len;              /* old allocated size */
        cache_len = needed;

        /* Fill in any missing terms */
        idx_t start = (old_len == 0) ? 1 : old_len;
        for (idx_t i = start; i <= n; ++i) {
            if (i == 1) {                       /* initial value */
                cache[1] = 1;
                continue;
            }

            idx_t m = i - 1;                    /* the n in the recurrence */
            seq_t sum = 0;

            /* Enumerate all divisors d of m */
            for (idx_t d = 1; d <= m / d; ++d) {
                if (m % d != 0)
                    continue;

                idx_t q = m / d;                /* paired divisor */

                /* contribution from divisor d */
                seq_t pow1 = pow_uint(4, q - 1);
                sum += cache[d] * pow1;

                if (q != d) {                  /* contribution from q, if distinct */
                    seq_t pow2 = pow_uint(4, d - 1);
                    sum += cache[q] * pow2;
                }
            }

            cache[i] = sum;
        }
    }

    return cache[n];
}
