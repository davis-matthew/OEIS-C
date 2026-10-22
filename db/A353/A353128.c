/*
 * A353128  Antidiagonal sums of A353109.
 * https://oeis.org/A353128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=0,a(1)=0,a(2)=1,a(3)=4,a(4)=10,a(5)=20,a(6)=35,a(7)=20,a(8)=39,a(9)=48,
 *            *
 *            a(10)=57,a(11)=40,a(12)=61,a(13)=58,a(14)=68,a(15)=92,a(16)=59,a(17)=96,a(18)=105,
 *            * a(n)=2*a(n-9)-a(n-18) for n>18
 * Range:     u8 n <= 41, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t curmax = (idx_t)-1;
    static const seq_t init_vals[19] = {
        0,0,1,4,10,20,35,20,39,48,57,40,61,58,68,92,59,96,105
    };

    if (cache == NULL) {
        /* initial capacity: at least 32 entries */
        cap = 32;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        for (size_t i = 0; i < 19; ++i) cache[i] = init_vals[i];
        curmax = 18;
    }

    if (n <= curmax) {
        return cache[(size_t)n];
    }

    /* extend cache if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cap = new_cap;
    }

    for (idx_t i = curmax + 1; i <= n; ++i) {
        seq_t t = cache[i - 9];
        t += t;                  /* t = 2 * cache[i-9] */
        t -= cache[i - 18];      /* t = 2*cache[i-9] - cache[i-18] */
        cache[i] = t;
    }
    curmax = n;
    return cache[(size_t)n];
}
