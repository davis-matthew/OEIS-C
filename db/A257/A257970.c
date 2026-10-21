/*
 * A257970  a(1) = 1, a(2) = 2, a(3) = 5; thereafter a(n) = 2 * Sum_{k=1..n-1}
 *          a(k).
 * https://oeis.org/A257970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(1)=1, a(2)=2, a(3)=5, a(4)=16, a(n)=3*a(n-1) for n>=5
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle small n directly */
    if (n <= 4) {
        switch (n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)2;
            case 3: return (seq_t)5;
            case 4: return (seq_t)16;
            default: return (seq_t)0; /* n==0 or other */
        }
    }

    /* Cache computed values for future calls */
    static seq_t *cache = NULL;
    static idx_t cap = 0;

    if (n > cap) {
        /* Grow cache to hold up to n */
        seq_t *tmp = (seq_t*)realloc(cache, n * sizeof(seq_t));
        if (!tmp) {
            /* Memory failure: compute on the fly without caching */
            seq_t a1 = (seq_t)1;
            seq_t a2 = (seq_t)2;
            seq_t a3 = (seq_t)5;
            seq_t a4 = (seq_t)16;
            seq_t an = a4;
            for (idx_t i = 5; i <= n; ++i) {
                an = a4 * (seq_t)3;
                a1 = a2; a2 = a3; a3 = a4; a4 = an;
            }
            return an;
        }
        cache = tmp;
        /* Initialize newly allocated entries */
        for (idx_t i = cap + 1; i <= n; ++i) {
            if (i == 1)      cache[i-1] = (seq_t)1;
            else if (i == 2) cache[i-1] = (seq_t)2;
            else if (i == 3) cache[i-1] = (seq_t)5;
            else if (i == 4) cache[i-1] = (seq_t)16;
            else             cache[i-1] = cache[i-2] * (seq_t)3;
        }
        cap = n;
    }

    return cache[n-1];
}
