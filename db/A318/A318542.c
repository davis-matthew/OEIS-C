/*
 * A318542  Number of n X 5 0..1 arrays with every element unequal to 0, 1, 4
 *          or 6 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A318542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +14*a(n-3) +7*a(n-4) +33*a(n-5) -6*a(n-6) *
 *            +20*a(n-7) -48*a(n-8) -4*a(n-9) -32*a(n-10) * +30*a(n-12)
 *            +14*a(n-13) -8*a(n-14) -10*a(n-15) * +9*a(n-16) -3*a(n-17)
 *            -13*a(n-18) +9*a(n-19) * +7*a(n-20) -5*a(n-21) -a(n-22) +a(n-23)
 *            for n>24 * * Initial values: * a(1)=8, a(2)=8, a(3)=26, a...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* first 24 terms of the sequence (index 0 unused) */
static const seq_t init24[25] = {
    0ULL,               /* unused index 0 */
    (seq_t)8, (seq_t)8, (seq_t)26, (seq_t)78,
    (seq_t)247, (seq_t)784, (seq_t)2349, (seq_t)7191,
    (seq_t)22268, (seq_t)68384, (seq_t)210050, (seq_t)645832,
    (seq_t)1985105, (seq_t)6102355, (seq_t)18758291,
    (seq_t)57660242, (seq_t)177244228, (seq_t)544835790,
    (seq_t)1674781291, (seq_t)5148145759, (seq_t)15824995614,
    (seq_t)48644815851, (seq_t)149530340549, (seq_t)459644547764
};

/* coefficients of the recurrence: coeff[d] corresponds to a(n-d) */
static const int coeff[24] = {
    0,    /* dummy index 0 */
    1,    /* a(n-1) */
    0,    /* a(n-2) */
    14,   /* a(n-3) */
    7,    /* a(n-4) */
    33,   /* a(n-5) */
    -6,   /* a(n-6) */
    20,   /* a(n-7) */
    -48,  /* a(n-8) */
    -4,   /* a(n-9) */
    -32,  /* a(n-10) */
    0,    /* a(n-11) */
    30,   /* a(n-12) */
    14,   /* a(n-13) */
    -8,   /* a(n-14) */
    -10,  /* a(n-15) */
    9,    /* a(n-16) */
    -3,   /* a(n-17) */
    -13,  /* a(n-18) */
    9,    /* a(n-19) */
    7,    /* a(n-20) */
    -5,   /* a(n-21) */
    -1,   /* a(n-22) */
    1     /* a(n-23) */
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for offset 0 */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* highest computed index */
    static size_t cache_cap = 0;   /* allocated capacity (indices 0..cap-1) */

    /* enlarge cache if necessary (ensure capacity of at least 25 for first terms) */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)((n < 24) ? 25 : n + 1);
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */

        /* copy old values */
        if (cache) {
            for (size_t i = 0; i < cache_cap; ++i)
                tmp[i] = cache[i];
            free(cache);
        }
        cache = tmp;
        cache_cap = new_cap;

        /* initialise first 24 terms if not already done */
        if (cache_size < 24) {
            idx_t start = cache_size + 1;
            if (start > 24) start = 24;
            for (idx_t i = start; i <= 24; ++i)
                cache[i] = init24[i];
            cache_size = (24 > n) ? n : 24;
        }
    }

    /* compute new terms if needed */
    if (n > cache_size) {
        for (idx_t k = cache_size + 1; k <= n; ++k) {
            if (k <= 24) continue;  /* initial values already set */
            seq_t sum = (seq_t)0;
            for (int d = 1; d <= 23; ++d) {
                int c = coeff[d];
                if (c == 0) continue;
                seq_t mult = (seq_t)(c > 0 ? c : -c);
                seq_t term = cache[k - d] * mult;
                if (c > 0)
                    sum += term;
                else
                    sum -= term;
            }
            cache[k] = sum;
        }
        cache_size = n;
    }

    return cache[n];
}
