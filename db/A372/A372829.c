/*
 * A372829  a(n) = n! * Sum_{k=0..floor(n/2)} k! / (2*k)!.
 * https://oeis.org/A372829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A372829:
 *  a(n) = n! * Σ_{k=0..⌊n/2⌋} (k!)/(2k)!
 *  Equivalently: a(n) = Σ_{k=0..⌊n/2⌋} binomial(n,2k) * k! * (n-2k)!
 *
 *  Recurrence (D-finite):
 *       2*a(n) - 2*n*a(n-1) - n*a(n-2) + n*(n-2)*a(n-3) = 0
 *  Initial values: a(0)=1, a(1)=1, a(2)=3.
 *
 *  This implementation uses the recurrence to compute values in linear
 *  time and stores the results in a static cache so subsequent calls
 *  benefit from memoisation, irrespective of call order.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;       /* allocated array of seq_t values */
    static idx_t cache_len = 0;       /* number of values currently computed */

    /* ensure the cache can hold values up to n */
    idx_t needed = n + 1;            /* indices 0..n inclusive */
    if (cache == NULL) {
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
    } else if (cache_len < needed) {
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    }

    /* compute values that are not yet known */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)3;
        } else {
            /* use the recurrence:
             * a(n) = [ 2*n*a(n-1) + n*a(n-2) - n*(n-2)*a(n-3) ] / 2
             */
            seq_t term1 = (seq_t)i * cache[i-1];
            term1 = term1 * ((seq_t)2);

            seq_t term2 = (seq_t)i * cache[i-2];

            seq_t term3 = ((seq_t)i * (seq_t)(i-2)) * cache[i-3];

            cache[i] = (term1 + term2 - term3) / ((seq_t)2);
        }
    }
    cache_len = needed;  /* we now have values up to index n */

    return cache[n];
}
