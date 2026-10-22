/*
 * A352436  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/3)} binomial(n,3*k+1) *
 *          a(k) * a(n-3*k-1).
 * https://oeis.org/A352436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = Sum_{k=0..floor((n-1)/3)} binomial(n,3*k+1) *
 *            a(k) * a(n-3*k-1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static memoization cache */
    static seq_t *cache      = NULL;
    static size_t  cache_cap = 0;
    static idx_t   computed  = 0;          /* largest index cached so far */

    /* initialise cache */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        computed = 0;
    }

    /* grow cache if needed (double size strategy) */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* return already computed value */
    if (n <= computed) return cache[n];

    /* compute values from computed+1 up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        idx_t maxk = (i - 1) / 3;
        seq_t sum  = (seq_t)0;

        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t bin  = binomial(i, 3 * k + 1);
            seq_t prod = bin * cache[k];
            prod = prod * cache[i - 3 * k - 1];
            sum += prod;
        }
        cache[i] = sum;
    }

    computed = n;
    return cache[n];
}
