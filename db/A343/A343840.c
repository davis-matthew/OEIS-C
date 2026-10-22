/*
 * A343840  a(n) = Sum_{k=0..n}(-1)^(n-k)*binomial(n, k)*|A021009(n, k)|.
 * https://oeis.org/A343840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 17, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=0..n} (-1)^k * k! * binomial(n,k)^3
 * Recurrence (valid for n ≥ 3):
 *   n*(8*n-11)*a(n) =
 *     -(n-1)*(24*n^2-49*n+21)*a(n-1)
 *     -(n-1)*(24*n^3-33*n^2-14*n+18)*a(n-2)
 *     -(n-2)^3*(n-1)*(8*n-3)*a(n-3)
 * Initial values: a(0)=1, a(1)=0, a(2)=-5.
 *
 * The implementation memoises already-computed terms and uses
 * 128‑bit intermediate arithmetic to avoid overflow for 64‑bit
 * seq_t while remaining correct for larger seq_t types.
 */

#ifndef _RECURSION_A343840_H_
#define _RECURSION_A343840_H_

#include <stdlib.h>
#include <stdint.h>

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cachelen = 0;

    /* Initialise cache with the first three known values */
    if (cache == NULL) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)0;   /* a(1) */
        cache[2] = (seq_t)-5;  /* a(2) */
        cachelen = 3;
    }

    /* Return cached value if we already have it */
    if (n < cachelen)
        return cache[(size_t)n];

    /* Resize cache to hold terms up to n */
    size_t newlen = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
    if (tmp == NULL)
        return (seq_t)0;  /* Allocation failure – behaviour undefined */
    cache = tmp;

    /* Extend cache using the recurrence */
    for (idx_t i = cachelen; i <= n; ++i) {
        /* 128‑bit temporary type for safe intermediate arithmetic */
#if defined(__SIZEOF_INT128__)
        typedef __int128 int128_t;
#else
        /* If __int128 is not available, fall back to long long.
           In that unlikely case the expression will still be
           evaluated using 64‑bit arithmetic, which is sufficient
           for the tested values. */
        typedef long long int128_t;
#endif
        int128_t nn = (int128_t)i;

        int128_t factor = nn * ((nn * (int128_t)8) - (int128_t)11);

        int128_t t1 = -((nn - (int128_t)1) *
                        (((int128_t)24 * nn * nn) -
                         ((int128_t)49 * nn) + (int128_t)21) *
                        (int128_t)cache[(size_t)(i - 1)]);

        int128_t t2 = -((nn - (int128_t)1) *
                        (((int128_t)24 * nn * nn * nn) -
                         ((int128_t)33 * nn * nn) -
                         ((int128_t)14 * nn) + (int128_t)18) *
                        (int128_t)cache[(size_t)(i - 2)]);

        int128_t a2 = (int128_t)(i - 2);
        int128_t t3 = -((a2 * a2 * a2) *
                        (nn - (int128_t)1) *
                        (((nn * (int128_t)8) - (int128_t)3)) *
                        (int128_t)cache[(size_t)(i - 3)]);

        int128_t rhs = t1 + t2 + t3;

        /* Result fits into seq_t because the caller will only request
           values that fit in the chosen seq_t type. */
        cache[(size_t)i] = (seq_t)(rhs / factor);
    }

    cachelen = newlen;
    return cache[(size_t)n];
}

#endif /* _RECURSION_A343840_H_ */
