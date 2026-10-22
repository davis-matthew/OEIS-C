/*
 * A304151  Number of nX3 0..1 arrays with every element unequal to 2, 3, 4 or
 *          6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*a(n-1) +4*a(n-2) -14*a(n-4) -35*a(n-5) +11*a(n-6) +
 *              41*a(n-7) +53*a(n-8) +3*a(n-9) -116*a(n-10) -148*a(n-11) +
 *              62*a(n-12) +213*a(n-13) +137*a(n-14) +57*a(n-15) +18*a(n-16) +
 *              6*a(n-17) +2*a(n-18)   for n > 19
 * Initial values (a(1) ... a(19)):
 * 0, 5, 4, 42, 130, 464, 1722, 6378, 22939, 84079,
 * 306987, 1119981, 4087469, 14922974, 54464744,
 * 198801521, 725648052, 2648666277, 9667805493
 */

#include <stdlib.h>

static const seq_t init_terms[20] = {
    0ULL,          /* a(0) unused */
    0ULL,          /* a(1) */
    5ULL,          /* a(2) */
    4ULL,          /* a(3) */
    42ULL,         /* a(4) */
    130ULL,        /* a(5) */
    464ULL,        /* a(6) */
    1722ULL,       /* a(7) */
    6378ULL,       /* a(8) */
    22939ULL,      /* a(9) */
    84079ULL,      /* a(10) */
    306987ULL,     /* a(11) */
    1119981ULL,    /* a(12) */
    4087469ULL,    /* a(13) */
    14922974ULL,   /* a(14) */
    54464744ULL,   /* a(15) */
    198801521ULL,  /* a(16) */
    725648052ULL,  /* a(17) */
    2648666277ULL, /* a(18) */
    9667805493ULL  /* a(19) */
};

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* highest n computed and stored in cache */
    /* Allocate or expand cache to hold up to n */
    if (n > cached) {
        size_t new_cap = n + 1;   /* allocate with index 0..n */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure, return 0 as fallback */
        /* Initialize missing entries if expanding beyond cached */
        if (n >= 19) {
            /* ensure base terms up to 19 are set */
            for (idx_t i = cached + 1; i <= 19; ++i)
                cache[i] = init_terms[i];
            if (n > 19) {
                for (idx_t i = 20; i <= n; ++i) {
                    /* Recurrence uses only previous terms that are already computed */
                    seq_t s1 = 3*cache[i-1] + 4*cache[i-2] + 11*cache[i-6] +
                               41*cache[i-7] + 53*cache[i-8] + 3*cache[i-9] +
                               62*cache[i-12] + 213*cache[i-13] + 137*cache[i-14] +
                               57*cache[i-15] + 18*cache[i-16] + 6*cache[i-17] +
                               2*cache[i-18];
                    seq_t s2 = 14*cache[i-4] + 35*cache[i-5] +
                               116*cache[i-10] + 148*cache[i-11];
                    cache[i] = s1 - s2;   /* s1 >= s2 for all n > 19 */
                }
            } else {
                /* n <= 19, simply copy base terms up to n */
                for (idx_t i = cached + 1; i <= n; ++i)
                    cache[i] = init_terms[i];
            }
        } else {
            /* n < 19, just copy needed base terms */
            for (idx_t i = cached + 1; i <= n; ++i)
                cache[i] = init_terms[i];
        }
        cached = n;
    }
    return cache[n];
}
