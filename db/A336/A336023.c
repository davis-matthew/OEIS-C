/*
 * A336023  a(0) = a(1) = a(2) = 1; a(n) = (1/2) * Sum_{k=0..n-2}
 *          binomial(n-2,k) * a(k) * a(n-k-2).
 * https://oeis.org/A336023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = a(2) = 1; a(n) = (1/2) * Sum_{k=0..n-2}
 *            binomial(n-2,k) * a(k) * a(n-k-2) (n>=3)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 18, u64 n <= 29, u128 n <= 46,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is memoised; it uses the helper ensure_cache to allocate
   a static array that grows as needed. */

#include <stdint.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = 0;   /* last index that has a stored value */

    /* initialise base cases (a(0)=a(1)=a(2)=1) */
    if (last_computed < 2) {
        size_t need = 3;                 /* indices 0..2 */
        ensure_cache(need-1, &cache, &cache_cap); /* allocate at least 3 slots */
        cache[0] = cache[1] = cache[2] = 1;
        last_computed = 2;
    }

    /* grow cache so that cache[n] is valid */
    ensure_cache(n, &cache, &cache_cap);

    /* return early if already computed */
    if (n <= last_computed)
        return cache[n];

    /* compute successive values up to n */
    for (idx_t m = last_computed + 1; m <= n; ++m) {
        if (m <= 2) {                    /* keep base cases unchanged */
            cache[m] = 1;
            continue;
        }

        seq_t sum   = 0;
        seq_t binom = 1;                 /* binom(m-2,0) */

        idx_t lim = m - 2;
        for (idx_t k = 0; k <= lim; ++k) {
            /* term = binom * a[k] * a[m-k-2] */
            seq_t tmp = cache[k];
            tmp *= cache[m - k - 2];
            tmp *= binom;
            sum += tmp;

            /* update binom for next k: binom(m-2,k+1) */
            if (k < lim) {
                binom *= (seq_t)(m - 2 - k);
                binom /= (seq_t)(k + 1);
            }
        }

        cache[m] = sum / 2;              /* integer division, sequence is even */
    }

    last_computed = n;
    return cache[n];
}
