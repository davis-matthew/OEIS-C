/*
 * A365972  Expansion of e.g.f. exp( Sum_{k>=0} x^(5*k+2) / (5*k+2) ).
 * https://oeis.org/A365972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 0; a(n) = (n-1)! * \sum_{k=0..⌊(n-2)/5⌋}
 *            a(n-5k-2) / (n-5k-2)! for n ≥ 2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache   = NULL;      /* array of computed a(i) */
    static size_t  cache_size = 0;    /* allocated number of entries */

    /* enlarge cache if necessary */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();      /* allocation failure – abort */
        cache = tmp;

        /* compute values from previous size up to n inclusive */
        idx_t i = (idx_t)cache_size;
        for (; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }
            if (i == 1) {
                cache[1] = (seq_t)0;
                continue;
            }
            /* product = ∏_{t=j+1}^{i-1} t   for current j (initially j=i-2)   */
            idx_t j = i - 2;
            seq_t sum = 0;
            seq_t prod = (seq_t)(i - 1);    /* when j = i-2, prod = product_{t=i-1}^{i-1} t = i-1 */

            for (;;) {
                sum += cache[j] * prod;
                if (j < 5) break;          /* next j would be negative */
                j -= 5;                     /* move to next term in the sum */

                /* multiply prod by the new five factors (j+1 … j+5) */
                for (idx_t t = j + 1; t <= j + 5; ++t)
                    prod *= (seq_t)t;
            }
            cache[i] = sum;
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
