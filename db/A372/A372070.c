/*
 * A372070  Triangular numbers that are 2 mod 4.
 * https://oeis.org/A372070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=6, a(2)=10, a(3)=66, a(4)=78, a(5)=190 a(n) =
 *            a(n-1)+2*a(n-2)-2*a(n-3)-a(n-4)+a(n-5) for n>5
 * Range:     u8 n <= 6, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* (seq_t and idx_t are provided by the build system) */
seq_t a(idx_t n)
{
    /* Calls may come in any order; cache previous results */
    static seq_t *cache = NULL;      /* array a[1]..a[max_computed] */
    static idx_t max_computed = 0;   /* largest index already computed */
    static size_t cache_cap = 0;     /* allocated size of cache */

    if (n == 0) return (seq_t)0;     /* not defined but harmless */

    /* Allocate or grow the cache if needed */
    if (n > max_computed) {
        size_t needed = (size_t)n;
        if (cache_cap < needed) {
            if (cache == NULL) {
                cache = (seq_t *)malloc(needed * sizeof(seq_t));
            } else {
                cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
            }
            cache_cap = needed;
            if (cache == NULL) abort();   /* allocation failed */
        }

        /* Compute new terms up to n */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1)              val = (seq_t)6;
            else if (i == 2)         val = (seq_t)10;
            else if (i == 3)         val = (seq_t)66;
            else if (i == 4)         val = (seq_t)78;
            else if (i == 5)         val = (seq_t)190;
            else {
                seq_t pos = cache[i - 2]              /* a(i-1) */
                          + ((seq_t)2 * cache[i - 3])    /* 2*a(i-2) */
                          + cache[i - 6];               /* +a(i-5) */
                seq_t neg = ((seq_t)2 * cache[i - 4]) + cache[i - 5];
                val = pos - neg;
            }
            cache[i - 1] = val;   /* store a(i) */
        }
        max_computed = n;
    }
    return cache[n - 1];
}
