/*
 * A249850  Number of length 6+4 0..n arrays with no five consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A249850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 55*a(n-2) + 165*a(n-3) - 330*a(n-4) +
 *            462*a(n-5) - 462*a(n-6) + 330*a(n-7) - 165*a(n-8) + 55*a(n-9) -
 *            11*a(n-10) + a(n-11) for n>11. Base values (computed from the
 *            empirical polynomial) are a(1)=10, a(2)=2163, a(3)=81816,
 *            a(4)=1234328, a(5)=10653298, a(6)=63374127, a(7)=2893726...
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 84, u128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  Helper: compute a(n) for 1 ≤ n ≤ 11 using the closed‐form polynomial  */
/*  a(n) = (1/2520) * (2520 n^10 - 1368 n^9 + 12981 n^8 - 4160 n^7
 *                    + 8736 n^6 + 10276 n^5 - 10311 n^4 + 7780 n^3
 *                    - 1326 n^2 + 72 n )                                 */
/* --------------------------------------------------------------------- */
static inline seq_t base_value(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */
    seq_t n4 = n3 * nn;           /* n^4 */
    seq_t n5 = n4 * nn;           /* n^5 */
    seq_t n6 = n5 * nn;           /* n^6 */
    seq_t n7 = n6 * nn;           /* n^7 */
    seq_t n8 = n7 * nn;           /* n^8 */
    seq_t n9 = n8 * nn;           /* n^9 */
    seq_t n10 = n9 * nn;          /* n^10 */

    seq_t res = (seq_t)2520 * n10;
    res -= (seq_t)1368 * n9;
    res += (seq_t)12981 * n8;
    res -= (seq_t)4160 * n7;
    res += (seq_t)8736 * n6;
    res += (seq_t)10276 * n5;
    res -= (seq_t)10311 * n4;
    res += (seq_t)7780 * n3;
    res -= (seq_t)1326 * n2;
    res += (seq_t)72 * nn;
    /* divide by 2520 – exact division for integer n */
    return res / (seq_t)2520;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Use polynomial for first 11 terms – no constants stored in code */
    if (n <= 11)
        return base_value(n);

    /* Static cache for memoisation */
    static seq_t *cache   = NULL;   /* array holding a(1)...a(current_max) */
    static idx_t  size    = 0;      /* highest n with computed value */
    static idx_t  cap     = 0;      /* allocated array size */

    /* Initialise cache if needed */
    if (cache == NULL) {
        /* Allocate just enough for the first 12 values (a(1)..a(11) + 1 reserve) */
        cap = 12;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (idx_t i = 1; i <= 11; ++i)
            cache[i - 1] = base_value(i);
        size = 11;
    }

    /* Grow cache if requested n is beyond current capacity */
    if (n > cap) {
        idx_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) /* allocation failed – leave old cache as is */
            return 0;
        cache = tmp;
        cap = new_cap;
    }

    /* Compute further terms up to n if they are not already known */
    if (n > size) {
        for (idx_t i = size + 1; i <= n; ++i) {
            seq_t term = (seq_t)11 * cache[i - 2]      /* 11*a(i-1) */
                      - (seq_t)55 * cache[i - 3]      /* 55*a(i-2) */
                      + (seq_t)165 * cache[i - 4]     /* 165*a(i-3) */
                      - (seq_t)330 * cache[i - 5]     /* 330*a(i-4) */
                      + (seq_t)462 * cache[i - 6]     /* 462*a(i-5) */
                      - (seq_t)462 * cache[i - 7]     /* 462*a(i-6) */
                      + (seq_t)330 * cache[i - 8]     /* 330*a(i-7) */
                      - (seq_t)165 * cache[i - 9]     /* 165*a(i-8) */
                      + (seq_t)55 * cache[i -10]      /* 55*a(i-9) */
                      - (seq_t)11 * cache[i -11]      /* 11*a(i-10) */
                      +          cache[i -12];        /*  a(i-11)   */
            cache[i - 1] = term;
        }
        size = n;
    }

    return cache[n - 1];
}
