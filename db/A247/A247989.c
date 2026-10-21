/*
 * A247989  Number of length n+5 0..2 arrays with no disjoint triples in any
 *          consecutive six terms having the same sum.
 * https://oeis.org/A247989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) +10*a(n-6) -20*a(n-7) -16*a(n-12) +32*a(n-13);
 *            Initial values (n=1..13): a(1)=396, a(2)=702, a(3)=1264,
 *            a(4)=2314, a(5)=4308, a(6)=8150, a(7)=15640, a(8)=30620,
 *            a(9)=60192, a(10)=118756, a(11)=235048, a(12)=466476,
 *            a(13)=927792
 * Range:     u8 none, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cap = 0;

    /* sequence is defined for n >= 1 only */
    if (n == 0) return 0;

    /* expand cache if necessary */
    if (n > cap) {
        /* first time: fill initial 13 terms */
        if (cap == 0) {
            cache = (seq_t*)malloc(13 * sizeof(seq_t));
            if (!cache) abort();
            static const seq_t init[13] = {
                396, 702, 1264, 2314, 4308,
                8150, 15640, 30620, 60192, 118756,
                235048, 466476, 927792
            };
            memcpy(cache, init, 13 * sizeof(seq_t));
            cap = 13;
        }

        /* resize cache to hold up to n */
        if (n > cap) {
            seq_t *tmp = (seq_t*)realloc(cache, n * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            /* compute missing values using the recurrence */
            idx_t start = cap + 1;
            if (start < 14) start = 14;          /* index 14 corresponds to a(14) */
            for (idx_t i = start; i <= n; ++i) {
                /* recurrence:
                   a(i) = 2*a(i-1) +10*a(i-6) -20*a(i-7) -16*a(i-12) +32*a(i-13)
                */
                seq_t res = 2 * cache[i - 2] + 10 * cache[i - 7] + 32 * cache[i - 14];
                res -= 20 * cache[i - 8];
                res -= 16 * cache[i - 13];
                cache[i - 1] = res;
            }
            cap = n;
        }
    }

    return cache[n - 1];
}
