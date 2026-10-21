/*
 * A241940  Number of length 4+4 0..n arrays with no consecutive five elements
 *          summing to more than 2*n.
 * https://oeis.org/A241940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9), for n>9;
 *            Initial values: a(1)=71, a(2)=1187, a(3)=9500, a(4)=49355,
 *            a(5)=193179, a(6)=619132, a(7)=1710198, a(8)=4211175,
 *            a(9)=9462805
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 20, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[10] = {
    0,          /* a(0) unused */
    71,         /* a(1) */
    1187,       /* a(2) */
    9500,       /* a(3) */
    49355,      /* a(4) */
    193179,     /* a(5) */
    619132,     /* a(6) */
    1710198,    /* a(7) */
    4211175,    /* a(8) */
    9462805     /* a(9) */
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* out of range, should not happen */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;   /* largest n for which cache[n] is valid */

    if (n > max_computed) {
        /* grow the cache to accommodate index n */
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) exit(1);  /* allocation failure */
        cache = new_cache;

        /* initialise newly added positions that are part of the initial block */
        for (idx_t i = max_computed + 1; i <= n && i <= 9; ++i) {
            cache[i] = init[i];
        }

        /* compute new terms using the recurrence */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i <= 9) continue;   /* already initialised */

            seq_t pos = 0, neg = 0;

            pos += cache[i - 1] * (seq_t)9;
            neg += cache[i - 2] * (seq_t)36;
            pos += cache[i - 3] * (seq_t)84;
            neg += cache[i - 4] * (seq_t)126;
            pos += cache[i - 5] * (seq_t)126;
            neg += cache[i - 6] * (seq_t)84;
            pos += cache[i - 7] * (seq_t)36;
            neg += cache[i - 8] * (seq_t)9;
            pos += cache[i - 9];

            cache[i] = pos - neg;   /* pos >= neg for all n≥10 */
        }

        max_computed = n;
    }

    return cache[n];
}
