/*
 * A360888  G.f. satisfies A(x) = 1 + x * (1 + x^2)^2 * A(x * (1 + x^2)).
 * https://oeis.org/A360888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A360888 : a(0)=1 and
   a(n) = Σ_{k=0..⌊(n−1)/2⌋} binomial(n+1−2k , k) * a(n−1−2k)  */

/*  The binomial function is provided in ../utils/binomial.h. */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache = NULL;          /* array of already computed a(i)   */
    static idx_t  cached_len = 0;        /* number of valid entries in cache */

    /* expand the cache if needed, keeping old length in old_len */
    if (n >= cached_len) {
        size_t old_len = cached_len;                    /* old size */
        size_t new_len = (size_t)n + 1;                 /* must hold indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;                             /* allocation failed */
        cache = tmp;
        cached_len = new_len;                           /* new size */

        /* compute missing values from old_len up to n inclusive */
        for (idx_t i = old_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;                           /* base case */
                continue;
            }
            idx_t maxk = (i - 1) / 2;
            seq_t sum = 0;
            for (idx_t k = 0; k <= maxk; ++k) {
                seq_t bin = binomial(i + 1 - 2 * k, k);  /* C(i+1-2k , k)   */
                seq_t term = bin * cache[i - 1 - 2 * k];
                sum += term;
            }
            cache[i] = sum;
        }
    }
    /* return the requested element */
    return cache[n];
}
