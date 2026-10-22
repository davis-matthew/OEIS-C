/*
 * A389992  Expansion of e.g.f. exp( x/(1-x)^2 ) / (1-x)^3.
 * https://oeis.org/A389992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+1)*a(n-1) - (n-1)*(3*n-1)*a(n-2) +
 *            (n-2)*(n-1)*n*a(n-3) * Initial values: a(0)=1, a(1)=4, a(2)=23
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t  cache_len = 0;

    /* The cache must hold indices up to n, inclusive */
    size_t required = (size_t)n + 1;
    if (required < 3) required = 3;  /* keep the three seed values */

    if (required > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!tmp) abort();                /* out of memory */
        cache = tmp;

        /* initialise the seed values if the cache is new */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)4;
            cache[2] = (seq_t)23;
        } else {
            /* if the cache has grown, make sure the earlier seeds are still set */
            if (cache_len < 2) cache[1] = (seq_t)4;
            if (cache_len < 3) cache[2] = (seq_t)23;
        }

        /* compute terms up to the required index */
        for (size_t i = cache_len; i < required; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)4;
            } else if (i == 2) {
                cache[i] = (seq_t)23;
            } else {
                seq_t nsi = (seq_t)i;                   /* i as seq_t */

                seq_t term1 = ((seq_t)3 * nsi + (seq_t)1) * cache[i-1];
                seq_t term2 = (nsi - (seq_t)1) *
                              ((seq_t)3 * nsi - (seq_t)1) *
                              cache[i-2];
                seq_t term3 = (nsi - (seq_t)2) *
                              (nsi - (seq_t)1) *
                              nsi *
                              cache[i-3];

                cache[i] = term1 - term2 + term3;
            }
        }
        cache_len = required;
    }

    return cache[(size_t)n];
}
