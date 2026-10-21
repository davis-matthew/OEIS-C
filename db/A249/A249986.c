/*
 * A249986  Number of length 6+1 0..2*n arrays with the sum of the absolute
 *          values of adjacent differences equal to 6*n.
 * https://oeis.org/A249986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7) for n>7; initial values: a(1)=466,
 *            a(2)=7138, a(3)=43068, a(4)=168506, a(5)=508902, a(6)=1290856,
 *            a(7)=2886016
 * Range:     u8 none, u16 n <= 3, u32 n <= 26, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* not part of the sequence but safe */

    static idx_t cur = 0;            /* number of precalculated terms */
    static seq_t *cache = NULL;      /* array holding terms a(1) .. a(cur) */

    /* initialize base values on the first call */
    if (cur == 0) {
        cur = 7;
        cache = (seq_t *)malloc(cur * sizeof(seq_t));
        cache[0] = 466;
        cache[1] = 7138;
        cache[2] = 43068;
        cache[3] = 168506;
        cache[4] = 508902;
        cache[5] = 1290856;
        cache[6] = 2886016;
    }

    /* extend the cache up to request n */
    if (n > cur) {
        cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        for (idx_t i = cur; i < n; ++i) {
            /* recurrence: a(i+1) = ... where i indexes 0-based cache */
            seq_t t1 = (seq_t)7 * cache[i-1];
            seq_t t2 = (seq_t)21 * cache[i-2];
            seq_t t3 = (seq_t)35 * cache[i-3];
            seq_t t4 = (seq_t)35 * cache[i-4];
            seq_t t5 = (seq_t)21 * cache[i-5];
            seq_t t6 = (seq_t)7 * cache[i-6];
            cache[i] = t1 - t2 + t3 - t4 + t5 - t6 + cache[i-7];
        }
        cur = n;
    }

    return cache[n-1];  /* sequence is 1‑indexed */
}
