/*
 * A352437  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/4)} binomial(n,4*k+1) *
 *          a(k) * a(n-4*k-1).
 * https://oeis.org/A352437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/4)} binomial(n,4*k+1) * a(k) *
 *            a(n-4*k-1) Initial values: a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Grow cache to accommodate n */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 4;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        /* In a contest environment we assume realloc succeeds */
        cache = tmp;
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }

    /* Initialise a(0) */
    if (cache[0] == 0) cache[0] = (seq_t)1;

    if (n == 0) return cache[0];

    /* Compute a(i) for all i ≤ n that are still 0 (uncomputed) */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != 0) continue;          /* already computed */

        idx_t maxk = (i - 1) / 4;
        seq_t sum = 0;

        /* r = 4*k + 1, start with k=0 -> r=1, binom = C(i,1) = i */
        idx_t r = 1;
        seq_t binom = (seq_t)i;                /* C(i,1) */

        for (idx_t k = 0; k <= maxk; ++k) {
            /* Accumulate the term */
            seq_t term = binom * cache[k] * cache[i - r];
            sum += term;

            /* Advance binom to C(i, r+4) for the next k (if any) */
            for (int step = 0; step < 4; ++step) {
                /* binom = binom * (i - r) / (r + 1) */
                seq_t num = (seq_t)(i - r);
                seq_t den = (seq_t)(r + 1);
                binom = (binom * num) / den;
                ++r;
            }
        }

        cache[i] = sum;
    }

    return cache[n];
}
