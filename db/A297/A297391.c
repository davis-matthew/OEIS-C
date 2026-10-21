/*
 * A297391  Number of n X 4 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 1 neighboring 1.
 * https://oeis.org/A297391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-2) + 4*a(n-3) - 3*a(n-4) - a(n-5) -
 *            2*a(n-6) - a(n-7); a(1)=4, a(2)=13, a(3)=37, a(4)=126, a(5)=385,
 *            a(6)=1243, a(7)=3924
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial seven terms */
    static const seq_t init[7] = {
        (seq_t)4, (seq_t)13, (seq_t)37, (seq_t)126,
        (seq_t)385, (seq_t)1243, (seq_t)3924
    };

    if (n <= 7) return init[n-1];

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (cache == NULL) {                 /* first call */
        cache_cap = 7;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        for (size_t i = 0; i < 7; ++i) cache[i] = init[i];
    }

    if ((size_t)n > cache_cap) {         /* extend cache */
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;

        for (size_t i = cache_cap; i < new_cap; ++i) {
            seq_t t1 = cache[i-1];
            seq_t t2 = cache[i-2];
            seq_t t3 = cache[i-3];
            seq_t t4 = cache[i-4];
            seq_t t5 = cache[i-5];
            seq_t t6 = cache[i-6];
            seq_t t7 = cache[i-7];

            seq_t pos = t1 + 6*t2 + 4*t3;
            seq_t neg = 3*t4 + t5 + 2*t6 + t7;
            cache[i] = pos - neg;
        }
        cache_cap = new_cap;
    }

    return cache[n-1];
}
