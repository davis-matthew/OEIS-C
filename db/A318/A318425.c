/*
 * A318425  Number of nX3 0..1 arrays with every element unequal to 1, 2, 3 or
 *          6 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A318425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For n > 20: a(n) = 9*a(n-2) + 11*a(n-3) - 14*a(n-4) - 32*a(n-5)
 *            - 29*a(n-6) - 51*a(n-7) + 33*a(n-8) + 116*a(n-9) + 18*a(n-10) +
 *            70*a(n-11) + 92*a(n-12) - 22*a(n-13) - 12*a(n-16) + 4*a(n-17)
 *            Initial values (a(1) .. a(24)): 1, 16, 36, 147, 417, 1353, 4095,
 *            12853, 39489, 123210, 380280, 1182026, 365...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Coefficients for the recurrence; coeff[j] is coefficient of a(n-j) */
static const int coeff[18] = {
    0,          /* index 0 unused */
    0,          /* a(n-1) */
    9,          /* a(n-2) */
    11,         /* a(n-3) */
    -14,        /* a(n-4) */
    -32,        /* a(n-5) */
    -29,        /* a(n-6) */
    -51,        /* a(n-7) */
    33,         /* a(n-8) */
    116,        /* a(n-9) */
    18,         /* a(n-10) */
    70,         /* a(n-11) */
    92,         /* a(n-12) */
    -22,        /* a(n-13) */
    0,          /* a(n-14) */
    0,          /* a(n-15) */
    -12,        /* a(n-16) */
    4           /* a(n-17) */
};

/* Initial terms a(1)..a(24) */
static const seq_t base[25] = {
    0ULL,              /* a(0) unused */
    1ULL,              /* a(1)  */
    16ULL,             /* a(2)  */
    36ULL,             /* a(3)  */
    147ULL,            /* a(4)  */
    417ULL,            /* a(5)  */
    1353ULL,           /* a(6)  */
    4095ULL,           /* a(7)  */
    12853ULL,          /* a(8)  */
    39489ULL,          /* a(9)  */
    123210ULL,         /* a(10) */
    380280ULL,         /* a(11) */
    1182026ULL,        /* a(12) */
    3659119ULL,        /* a(13) */
    11351061ULL,       /* a(14) */
    35179874ULL,       /* a(15) */
    109070933ULL,      /* a(16) */
    338113844ULL,      /* a(17) */
    1048217069ULL,     /* a(18) */
    3249453631ULL,     /* a(19) */
    10073853147ULL,    /* a(20) */
    31229167798ULL,    /* a(21) */
    96814146538ULL,    /* a(22) */
    300130174990ULL,   /* a(23) */
    930430509302ULL    /* a(24) */
};

/* Cache of computed terms.  cache[0] is unused. */
static seq_t *cache = NULL;
static idx_t  cache_cap = 0;

/* Return a(n) for n >= 1.  Undefined for n == 0; returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* safety guard */

    /* Extend cache if necessary */
    if (cache_cap < n) {
        idx_t new_cap = n;
        seq_t *new_cache = (seq_t*)malloc((new_cap + 1) * sizeof(seq_t));
        if (!new_cache) return 0;   /* allocation failure */

        /* copy existing entries */
        for (idx_t i = 1; i <= cache_cap; ++i)
            new_cache[i] = cache[i];

        /* compute missing entries */
        for (idx_t i = cache_cap + 1; i <= new_cap; ++i) {
            if (i <= 24) {
                new_cache[i] = base[i];   /* use given initial values */
            } else {
                /* compute using the recurrence */
                seq_t pos = 0;
                seq_t neg = 0;
                for (int j = 1; j <= 17; ++j) {
                    int c = coeff[j];
                    if (c == 0) continue;
                    seq_t term = new_cache[i - j];
                    seq_t mul  = term * (seq_t)(c > 0 ? c : -c);
                    if (c > 0) pos += mul;
                    else       neg += mul;
                }
                new_cache[i] = pos - neg;
            }
        }

        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }
    return cache[n];
}
