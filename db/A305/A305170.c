/*
 * A305170  Number of nX3 0..1 arrays with every element unequal to 2, 3, 4, 6
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1)+4*a(n-2)+3*a(n-3)
 *            -19*a(n-4)-42*a(n-5)+50*a(n-6)+44*a(n-7)
 *            -33*a(n-8)-9*a(n-9)-2*a(n-10)
 *            -107*a(n-11)-33*a(n-12)+116*a(n-13)
 *            +112*a(n-14)+80*a(n-15)+34*a(n-16)+16*a(n-17)+8*a(n-18) with
 *            a(1)=0, a(2)=5, a(3)=4, a(4)=46, a(5)=151, a(6)=543, a(7)=2120,
 *            a(8)=8155, a(9)=30205, a(10)=11...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[1..] holds values of a(n) */
static size_t cap   = 0;       /* allocated capacity (number of elements) */
static size_t len   = 0;       /* highest n that has been computed */

/* Ensure cache can accommodate indices 0 .. need (inclusive). */
static void ensure_capacity(idx_t need)
{
    size_t required = (size_t)need + 1;  /* +1 because we store 0..need */
    if (required <= cap)
        return;
    size_t newcap = cap ? cap : 64;
    while (newcap < required)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cap   = newcap;
}

/* Base values a(1) … a(18). */
static const seq_t init_vals[18] = {
    0ULL,          /* a(1) */
    5ULL,          /* a(2) */
    4ULL,          /* a(3) */
    46ULL,         /* a(4) */
    151ULL,        /* a(5) */
    543ULL,        /* a(6) */
    2120ULL,       /* a(7) */
    8155ULL,       /* a(8) */
    30205ULL,      /* a(9) */
    115292ULL,     /* a(10) */
    437367ULL,     /* a(11) */
    1652127ULL,    /* a(12) */
    6259372ULL,    /* a(13) */
    23720673ULL,   /* a(14) */
    89793169ULL,   /* a(15) */
    340068226ULL,  /* a(16) */
    1288031893ULL, /* a(17) */
    4877697803ULL  /* a(18) */
};

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      /* undefined for n < 1 */

    /* Lazy initialization of cache. */
    if (len == 0) {
        ensure_capacity(18);
        cache[0] = 0;          /* unused but defined */
        for (idx_t i = 1; i <= 18; ++i)
            cache[i] = init_vals[i - 1];
        len = 18;
    }

    /* If requested n is greater than the current range, extend the cache. */
    if ((size_t)n > len) {
        ensure_capacity(n);
        for (size_t m = len + 1; m <= (size_t)n; ++m) {
            seq_t v = 0;
            /* Positive contributions. */
            v += (seq_t) 3 * cache[m - 1];
            v += (seq_t) 4 * cache[m - 2];
            v += (seq_t) 3 * cache[m - 3];
            v += (seq_t)50 * cache[m - 6];
            v += (seq_t)44 * cache[m - 7];
            v += (seq_t)116 * cache[m - 13];
            v += (seq_t)112 * cache[m - 14];
            v += (seq_t)80  * cache[m - 15];
            v += (seq_t)34  * cache[m - 16];
            v += (seq_t)16  * cache[m - 17];
            v += (seq_t)8   * cache[m - 18];
            /* Negative contributions. */
            v -= (seq_t)19   * cache[m - 4];
            v -= (seq_t)42   * cache[m - 5];
            v -= (seq_t)33   * cache[m - 8];
            v -= (seq_t)9    * cache[m - 9];
            v -= (seq_t)2    * cache[m - 10];
            v -= (seq_t)107  * cache[m - 11];
            v -= (seq_t)33   * cache[m - 12];
            cache[m] = v;
        }
        len = n;
    }

    return cache[n];
}
