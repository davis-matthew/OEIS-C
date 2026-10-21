/*
 * A291736  p-INVERT of (1,0,1,0,0,0,0,...), where p(S) = 1 - S^2 - S^3.
 * https://oeis.org/A291736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+a(n-3)+2*a(n-4)+3*a(n-5)+a(n-6)+3*a(n-7)+a(n-9) (n
 *            >= 10) Initial values: a(0)=0, a(1)=1, a(2)=1, a(3)=3, a(4)=5,
 *            a(5)=7, a(6)=16, a(7)=22, a(8)=47, a(9)=73
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 40, u64 n <= 80, u128 n <= 159,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* number of terms currently stored */
    static size_t cache_cap = 0;   /* capacity of the allocated array */

    /* Direct return for the first 10 terms */
    if (n < 10) {
        switch (n) {
        case 0: return (seq_t)0;
        case 1: return (seq_t)1;
        case 2: return (seq_t)1;
        case 3: return (seq_t)3;
        case 4: return (seq_t)5;
        case 5: return (seq_t)7;
        case 6: return (seq_t)16;
        case 7: return (seq_t)22;
        case 8: return (seq_t)47;
        case 9: return (seq_t)73;
        }
    }

    /* Initialisation: create cache and fill the first 10 terms */
    if (cache == NULL) {
        cache_cap = 16;                     /* initial capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)7;
        cache[6] = (seq_t)16;
        cache[7] = (seq_t)22;
        cache[8] = (seq_t)47;
        cache[9] = (seq_t)73;
        cache_len = 10;                     /* terms 0 .. 9 stored */
    }

    /* Ensure the cache can hold terms up to n */
    if (n >= cache_len) {
        /* Grow capacity if necessary */
        if (cache_cap < (size_t)(n + 1)) {
            size_t new_cap = cache_cap;
            while (new_cap < (size_t)(n + 1))
                new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val = 0;
            val += cache[i - 2];
            val += cache[i - 3];
            val += cache[i - 4] * (seq_t)2;
            val += cache[i - 5] * (seq_t)3;
            val += cache[i - 6];
            val += cache[i - 7] * (seq_t)3;
            val += cache[i - 9];
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
