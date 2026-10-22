/*
 * A354728  E.g.f. A(x) satisfies A(x) = 1 + log(1+x) * A(log(1+x)).
 * https://oeis.org/A354728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} k * Stirling1(n,k) * a(k-1) with a(0) = 1;
 *            Stirling1(n,k) are signed Stirling numbers of the first kind
 * Range:     i8 n <= 6, i16 n <= 7, i32 n <= 11, i64 n <= 17, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache   = NULL;
    static size_t  cachesz = 0;      /* number of elements stored */
    static size_t  cachecap = 0;     /* allocated capacity */

    /* storage for Stirling numbers two rows */
    static seq_t *s[2] = {NULL, NULL};
    static size_t  scap  = 0;        /* allocated capacity of Stirling arrays */
    static int     slot   = 0;       /* 0 or 1 */

    /* initialize cache and first Stirling array on first call */
    if (cachesz == 0) {
        cachecap = 2;
        cache = (seq_t *)malloc(cachecap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;      /* a(0) */
        cachesz = 1;

        scap = 2;
        s[0] = (seq_t *)malloc(scap * sizeof(seq_t));
        s[1] = (seq_t *)malloc(scap * sizeof(seq_t));
        if (!s[0] || !s[1]) exit(1);
        s[0][0] = (seq_t)1;      /* Stirling numbers for n=0, s(0,0)=1 */
    }

    /* if the requested n is already computed, return it */
    if ((size_t)n < cachesz)
        return cache[n];

    /* Ensure cache capacity up to n */
    size_t need = (size_t)n + 1;
    if (need > cachecap) {
        size_t newcap = cachecap;
        while (newcap < need) newcap <<= 1;
        seq_t *p = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!p) exit(1);
        cache = p;
        cachecap = newcap;
    }

    /* Ensure Stirling arrays capacity up to n+1 */
    size_t needscap = (size_t)n + 2; /* need indices up to n inclusive */
    if (needscap > scap) {
        size_t newcap = scap;
        while (newcap < needscap) newcap <<= 1;
        seq_t *p0 = (seq_t *)realloc(s[0], newcap * sizeof(seq_t));
        seq_t *p1 = (seq_t *)realloc(s[1], newcap * sizeof(seq_t));
        if (!p0 || !p1) exit(1);
        s[0] = p0;
        s[1] = p1;
        scap = newcap;
    }

    /* Extend cache and compute Stirling numbers incrementally */
    size_t start = cachesz;          /* first n to compute */
    for (size_t curr = start; curr <= (size_t)n; ++curr) {
        seq_t *prev = s[slot];
        seq_t *currs = s[slot ^ 1];
        size_t i;

        currs[0] = (seq_t)0;
        /* compute s(curr, k) for 1 <= k < curr */
        for (i = 1; i < curr; ++i) {
            seq_t term1 = prev[i - 1];
            seq_t term2 = (seq_t)(curr - 1) * prev[i];
            currs[i] = term1 - term2;
        }
        /* last entry: s(curr, curr) = prev[curr-1] */
        currs[curr] = prev[curr - 1];

        /* compute a(curr) = sum_{k=1..curr} k * s(curr,k) * a(k-1) */
        seq_t sum = (seq_t)0;
        for (i = 1; i <= curr; ++i) {
            seq_t term = currs[i] * cache[i - 1];
            term *= (seq_t)i;
            sum += term;
        }
        cache[curr] = sum;

        /* swap slots for next iteration */
        slot ^= 1;
    }

    cachesz = need;                  /* cache now holds up to n */
    return cache[n];
}
