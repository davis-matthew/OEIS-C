/*
 * A385066  E.g.f. A(x) satisfies A(x) = exp(x + x^3*A''(x)).
 * https://oeis.org/A385066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + ∑_{k=0}^{n-1} (-k +
 *            k³)·binomial(n-1,k)·a(k)·a(n-1-k) with a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

/* Return the nth term of OEIS sequence A385066. */
seq_t a(idx_t n)
{
    /* memoisation cache */
    static seq_t *cache = NULL;     /* array of computed values   */
    static idx_t cached_size = 0;   /* number of values stored   */
    static idx_t cache_cap  = 0;    /* current allocated size    */

    /* grow the cache so that it can hold index n */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap <= n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0)     /* allocation failure */
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n if necessary */
    while (cached_size <= n) {
        idx_t i = cached_size;
        seq_t value;

        if (i <= 2) {             /* a(0)=a(1)=a(2)=1 */
            value = (seq_t)1;
        } else {
            seq_t sum = 0;
            seq_t prev = cache[i - 1];

            for (idx_t k = 0; k < i; ++k) {
                /* weight = (-k + k³) */
                seq_t k_seq = (seq_t)k;
                seq_t weight = k_seq * k_seq * k_seq - k_seq;
                if (weight == 0)
                    continue;      /* skip k=0,1 where weight=0 */

                seq_t bin = binomial(i - 1, k);
                seq_t term = weight * bin;
                term *= cache[k];
                term *= cache[i - 1 - k];
                sum += term;
            }
            value = prev + sum;
        }

        cache[i] = value;
        ++cached_size;
    }

    return cache[n];
}
