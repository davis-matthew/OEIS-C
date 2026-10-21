/*
 * A284383  a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 2; a(n) is the largest
 *          composite number <= (a(n-a(n-1)) + a(n-a(n-2))) for n > 4.
 * https://oeis.org/A284383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 2; for n > 4: a(n) =
 *            largest composite ≤ (a(n-a(n-1)) + a(n-a(n-2)))
 * Range:     u8 n <= 380, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Memoisation table: index 1..N stored in cache[1..N] */
static seq_t *cache       = NULL;
static idx_t  cache_cap   = 0;   /* allocated size (≥ cache_computed+1) */
static idx_t  cache_computed = 0;/* highest n already computed */

seq_t a(idx_t n)
{
    /* n is always ≥ 1; guard for 0 just in case */
    if (n == 0) return 0;

    /* grow the cache if necessary (C++ requires explicit cast) */
    if (cache_cap < n + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failed – return 0 */
        cache      = tmp;
        cache_cap  = n + 1;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = 1;
        } else if (i == 2) {
            cache[i] = 2;
        } else if (i == 3) {
            cache[i] = 3;
        } else if (i == 4) {
            cache[i] = 2;
        } else {
            /* recurrence indices (indices are 1‑based) */
            idx_t idx1 = i - (idx_t)cache[i - 1];
            idx_t idx2 = i - (idx_t)cache[i - 2];

            seq_t sum = cache[idx1] + cache[idx2];
            seq_t val = sum;

            /* find the largest composite ≤ sum */
            while (val > 1 && is_prime_big(val)) {
                val = val - (seq_t)1;
            }
            cache[i] = val;   /* will be composite or 0 if sum < 2 */
        }
    }
    cache_computed = n;
    return cache[n];
}
