/*
 * A373638  a(n) = Sum_{k=0..floor(n/2)} binomial(3*n-5*k,k).
 * https://oeis.org/A373638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 59, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n)=a(n-1)+3*a(n-2)-3*a(n-4)+a(n-6), 
   a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=9, a(5)=21 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = (idx_t)(-1);

    /* allocate or grow cache to hold value n */
    if (cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = (last_computed == (idx_t)(-1) ? 0 : last_computed + 1); i <= n; ++i) {
        if (i == 0)                      cache[0] = 1;
        else if (i == 1)                 cache[1] = 1;
        else if (i == 2)                 cache[2] = 2;
        else if (i == 3)                 cache[3] = 5;
        else if (i == 4)                 cache[4] = 9;
        else if (i == 5)                 cache[5] = 21;
        else {
            seq_t term1 = cache[i-1] + (seq_t)3 * cache[i-2] + cache[i-6];
            seq_t term2 = (seq_t)3 * cache[i-4];
            cache[i] = term1 - term2;
        }
    }
    last_computed = n;
    return cache[n];
}
