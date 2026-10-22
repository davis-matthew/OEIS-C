/*
 * A386453  a(0) = 1; a(n) = a(n-1) + Sum_{k=0..n-1} binomial(k+2,3) * a(k) *
 *          a(n-1-k).
 * https://oeis.org/A386453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = a(n-1) + Σ_{k=0}^{n-1} binomial(k+2,3) * a(k) *
 *            a(n-1-k) for n ≥ 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    /* highest index that has been computed so far */
    static size_t  computed = 0;

    /* enlarge cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);    /* out of memory */
        cache = tmp;
        /* initialise newly allocated slots to zero (safe for unsigned types) */
        for (size_t i = cap; i < newcap; ++i) cache[i] = 0;
        cap = newcap;
    }

    /* ensure a(0) is set once */
    if (computed == 0) {
        cache[0] = 1;
        computed = 0;
    }

    /* if the requested n is larger than anything we have computed, do it */
    if ((size_t)n > computed) {
        for (size_t i = computed + 1; i <= (size_t)n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 0; k <= (idx_t)(i - 1); ++k) {
                /* binomial(k+2,3) = (k+2)*(k+1)*k / 6 */
                seq_t bin = ((seq_t)k * (k + 1) * (k + 2)) / 6;
                seq_t prod = bin * cache[k] * cache[i - 1 - k];
                sum += prod;
            }
            cache[i] = cache[i - 1] + sum;
        }
        computed = (size_t)n;
    }

    return cache[n];
}
