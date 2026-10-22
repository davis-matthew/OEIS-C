/*
 * A330151  Partial sums of 4th powers of the even numbers.
 * https://oeis.org/A330151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} (2*k)^4 = 16*Sum_{k=1..n} k^4 Implementation
 *            uses the sixth‑order linear recurrence a(n) = 6*a(n-1) -
 *            15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) - a(n-6) with
 *            initial values a(0)=0, a(1)=16, a(2)=272, a(3)=1568, a(4)=5664,
 *            a(5)=15664
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 66, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of already computed terms */
static idx_t  cache_len = 0;         /* current number of stored terms */

static const seq_t init_vals[6] = {0, 16, 272, 1568, 5664, 15664};

seq_t a(idx_t n)
{
    /* If n is small, return from the list of initial values. */
    if (n < 6)
        return init_vals[n];

    /* Grow the cache if necessary. */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory – terminate */
        cache = tmp;
        cache_len = new_len;

        /* Fill the cache up to the requested index. */
        if (cache_len == new_len && new_len >= 6) {
            /* Ensure that the initial values are stored. */
            for (idx_t i = 0; i < 6; ++i)
                cache[i] = init_vals[i];
        }

        /* Compute terms from the last computed index onward. */
        for (idx_t i = 6; i <= n; ++i) {
            /* Compute the positive part: 6*a[i-1] + 20*a[i-3] + 6*a[i-5] */
            seq_t pos = 6 * cache[i - 1] + 20 * cache[i - 3] + 6 * cache[i - 5];
            /* Compute the negative part: 15*a[i-2] + 15*a[i-4] + a[i-6] */
            seq_t neg = 15 * cache[i - 2] + 15 * cache[i - 4] + cache[i - 6];
            cache[i] = pos - neg; /* pos >= neg for all n >= 6 */
        }
    }

    return cache[n];
}
