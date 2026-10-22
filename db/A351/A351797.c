/*
 * A351797  a(1) = 1; a(n+1) = -a(n) + 2 * Sum_{d|n} a(n/d) * a(d).
 * https://oeis.org/A351797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n+1) = -a(n) + 2 * Sum_{d|n} a(d) * a(n/d) for n >=
 *            1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache[0] is unused; cache[i] holds a(i) for i >= 1 */
static seq_t *cache = NULL;
static idx_t cache_size = 0;  /* highest index already computed */

/* Return the n-th term of OEIS sequence A351797 (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0U) return 0;  /* safety guard; not used by OEIS */

    /* Grow the cache if needed */
    if (cache_size < n) {
        size_t new_cap = (size_t)(n + 1);   /* keep index 0 unused */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* memory failure */
        cache = tmp;

        /* Compute missing values in order */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1) {                  /* initial value */
                cache[1] = (seq_t)1;
                continue;
            }

            /* Compute a(i) using the recurrence a(i) = -a(i-1) + 2 * Σ_{d|i-1} a(d)*a((i-1)/d) */
            idx_t m = i - 1;                /* m = i-1, the argument of the sum */
            seq_t sum = (seq_t)0;

            /* iterate over divisors of m */
            for (idx_t d = 1; d * d <= m; ++d) {
                if (m % d == 0) {
                    idx_t d2 = m / d;
                    seq_t prod = cache[d] * cache[d2];
                    if (d == d2)
                        sum += prod;              /* perfect square divisor counted once */
                    else
                        sum += prod + prod;        /* add twice for both d and d2 */
                }
            }

            /* a(i) = 2*sum - a(i-1) */
            cache[i] = (seq_t)2 * sum - cache[m];
        }
        cache_size = n;
    }
    return cache[n];
}
