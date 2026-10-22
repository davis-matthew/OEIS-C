/*
 * A316871  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 5, 6 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - a(n-2) + 2*a(n-3) -36*a(n-4) -22*a(n-5) *
 *            +16*a(n-6) +131*a(n-7) +27*a(n-8) -72*a(n-9) -71*a(n-10) *
 *            +21*a(n-11) +16*a(n-12) -15*a(n-13) -10*a(n-14) -18*a(n-15) *
 *            -8*a(n-16) , for n > 17 * Initial values (n = 1 .. 17): * 4, 23,
 *            99, 425, 1877, 8333, 36900, 163433, 724406, * 32109...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*  recurrence coefficients for a(n-1) … a(n-16)  */
static const int coeff[16] = {
    5,  -1,   2, -36, -22,  16, 131,  27,
   -72, -71,  21,  16, -15, -10, -18,  -8
};

/*  initial 17 values (index 0 unused)  */
static const seq_t init_vals[18] = {
    0ULL,
    (seq_t)4ULL,        (seq_t)23ULL,     (seq_t)99ULL,      (seq_t)425ULL,
    (seq_t)1877ULL,     (seq_t)8333ULL,   (seq_t)36900ULL,   (seq_t)163433ULL,
    (seq_t)724406ULL,   (seq_t)3210961ULL,(seq_t)14232174ULL,(seq_t)63085590ULL,
    (seq_t)279639102ULL,(seq_t)1239553378ULL,(seq_t)5494572826ULL,
    (seq_t)24355881186ULL,(seq_t)107962769031ULL
};

/*  cache of computed terms  */
static seq_t *cache = NULL;         /* indices 0 .. cache_cap-1  */
static size_t cache_cap = 0;        /* allocated size of cache     */
static size_t cache_last = 0;       /* highest n that has been computed (n >= 1) */

/*  grow cache so that it contains at least indices 0..needed-1   */
static void
grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;

    /*  realloc returns void* in C++  */
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp)
        abort();          /*  allocation failure  */

    /*  zero the newly added part  */
    if (needed > cache_cap) {
        size_t i;
        for (i = cache_cap; i < needed; ++i)
            tmp[i] = 0;
    }

    cache = tmp;
    cache_cap = needed;

    /*  fill in the fixed initial values if they fit  */
    if (cache_cap > 1) {
        size_t i;
        for (i = 1; i < 18 && i < cache_cap; ++i)
            cache[i] = init_vals[i];
    }
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /*  defined only for n >= 1  */

    /*  ensure cache large enough  */
    grow_cache((size_t)n + 1);

    /*  compute missing terms up to n  */
    if (cache_last < n) {
        size_t i;
        for (i = cache_last + 1; i <= (size_t)n; ++i) {
            if (i < 18) {          /*  copy known initial terms  */
                cache[i] = init_vals[i];
                continue;
            }

            /*  recurrence  */
            seq_t pos = 0, neg = 0;
            size_t j;
            for (j = 1; j <= 16; ++j) {
                seq_t term = cache[i - j];
                int c = coeff[j - 1];
                if (c >= 0)
                    pos += term * (seq_t)c;
                else
                    neg += term * (seq_t)(-c);
            }
            cache[i] = pos - neg;  /*  pos >= neg  */
        }
        cache_last = (size_t)n;
    }

    return cache[(size_t)n];
}
