/*
 * A291415  p-INVERT of (1,1,0,0,0,0,...), where p(S) = 1 - 3 S + S^2.
 * https://oeis.org/A291415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=3, a(1)=11, a(2)=37, a(3)=126, a(4)=427
   a(n) = 3*a(n-1) + 2*a(n-2) - 2*a(n-3) - a(n-4)  for n ≥ 5 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Initialize cache with the first five terms */
    if (!cache) {
        cache_cap = 16;                    /* reasonable starting capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failure */
        cache_len = 5;
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)37;
        cache[3] = (seq_t)126;
        cache[4] = (seq_t)427;
    }

    if (n < cache_len)
        return cache[n];

    /* Grow cache to hold terms up to n */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap <= n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* realloc failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i < 5) {
            switch (i) {
                case 0: val = (seq_t)3; break;
                case 1: val = (seq_t)11; break;
                case 2: val = (seq_t)37; break;
                case 3: val = (seq_t)126; break;
                case 4: val = (seq_t)427; break;
            }
        } else {
            seq_t t1 = 3 * cache[i - 1];
            seq_t t2 = 2 * cache[i - 2];
            val   = t1 + t2;
            val  -= 2 * cache[i - 3];
            val  -= cache[i - 4];
        }
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
