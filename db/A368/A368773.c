/*
 * A368773  Antidiagonal sums of A059450.
 * https://oeis.org/A368773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence (n+1)*a(n) - 3*a(n-1) +
 *            (10*n-11)*a(n-2) + 3*a(n-3) + 9*(n-4)*a(n-4) = 0 with initial
 *            values a(0)=1, a(1)=1, a(2)=3, a(3)=7
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* Return cached value if already computed */
    if ((size_t)n < cap) {
        return cache[n];
    }

    /* Grow cache to n+1 entries */
    size_t old_cap = cap;
    cap = (size_t)n + 1;
    if (cache == NULL) {
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
    } else {
        cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
    }

    /* Compute missing terms */
    for (idx_t i = old_cap; i <= n; ++i) {
        switch (i) {
            case 0: cache[i] = 1; break;
            case 1: cache[i] = 1; break;
            case 2: cache[i] = 3; break;
            case 3: cache[i] = 7; break;
            default: {
                /* Compute the numerator of the recurrence */
                seq_t term1 = 3 * cache[i-1];
                seq_t term2 = ( (seq_t)10 * (seq_t)i - (seq_t)11 ) * cache[i-2];
                seq_t term3 = 3 * cache[i-3];
                seq_t term4 = 9 * ( (seq_t)i - (seq_t)4 ) * cache[i-4];

                seq_t num = term1 + term2;
                num -= term3;
                num -= term4;

                cache[i] = num / ( (seq_t)i + (seq_t)1 );
            }
        }
    }

    return cache[n];
}
