/*
 * A273053  Numbers n such that 15*n^2 + 16 is a square.
 * https://oeis.org/A273053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=4; a(n)=8*a(n-1)-a(n-2) for n>2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A273053 – numbers n with 15*n^2+16 a perfect square. */

#include <stdlib.h>

/* The function must compile as both C11 and C++17.  Casting the
   result of realloc to the appropriate type works in both languages. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* behaviour below the offset is irrelevant */

    static seq_t *cache = NULL;        /* 0‑based array: cache[0]=a(1) ... */
    static size_t len  = 0;            /* number of terms currently stored   */
    static size_t cap  = 0;            /* current allocated capacity         */

    /* initialise base cases the first time we are called */
    if (len < 2) {
        if (cap < 2) {
            cap = 2;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }
        cache[0] = (seq_t)0;           /* a(1) */
        cache[1] = (seq_t)4;           /* a(2) */
        len = 2;
    }

    /* grow the cache up to at least n */
    if ((size_t)n > len) {
        size_t newlen = (size_t)n;

        if (newlen > cap) {
            size_t newcap = cap ? cap * 2 : 4;
            while (newcap < newlen) newcap *= 2;
            cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            cap = newcap;
        }

        /* compute the missing terms */
        for (size_t i = len; i < newlen; ++i) {
            cache[i] = cache[i-1] * (seq_t)8 - cache[i-2];
        }
        len = newlen;
    }

    return cache[n-1];
}
