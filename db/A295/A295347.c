/*
 * A295347  Number of nX3 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0, 2 or 3 1s.
 * https://oeis.org/A295347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A295347
   Number of n×3 0..1 arrays with each 1 horizontally or vertically adjacent to 0, 2 or 3 1s.
   Computed via the 18‑term linear recurrence:
   a(n) =  6*a(n-1) - 4*a(n-2) -13*a(n-3) - 8*a(n-4) -17*a(n-5)
           +35*a(n-6) -14*a(n-7) +43*a(n-8) +48*a(n-9) -41*a(n-10)
           +43*a(n-11) -25*a(n-12) - 5*a(n-13) -18*a(n-14) - 1*a(n-15)
           + 2*a(n-16) + 1*a(n-17) + 1*a(n-18)
   with initial values a(1)…a(18) given below.  */

/* ----------  begin function implementation  ---------- */

#include <stdlib.h>
#include <stdint.h>

/* initial values a(1) … a(18) */
static const seq_t init_vals[18] = {
    (seq_t)  5ULL,           /* a(1) */
    (seq_t) 20ULL,           /* a(2) */
    (seq_t) 76ULL,           /* a(3) */
    (seq_t)299ULL,           /* a(4) */
    (seq_t)1198ULL,          /* a(5) */
    (seq_t)4775ULL,          /* a(6) */
    (seq_t)19214ULL,         /* a(7) */
    (seq_t)77650ULL,         /* a(8) */
    (seq_t)314916ULL,        /* a(9) */
    (seq_t)1281050ULL,       /* a(10) */
    (seq_t)5224082ULL,       /* a(11) */
    (seq_t)21344769ULL,      /* a(12) */
    (seq_t)87348360ULL,      /* a(13) */
    (seq_t)357898094ULL,     /* a(14) */
    (seq_t)1467894633ULL,    /* a(15) */
    (seq_t)6025215866ULL,    /* a(16) */
    (seq_t)24746954977ULL,   /* a(17) */
    (seq_t)101691771957ULL   /* a(18) */
};

/* recurrence coefficients  (a(n-1) … a(n-18) terms) */
static const int coeff[18] = {
     6, -4, -13, -8, -17, 35, -14, 43, 48, -41, 43, -25, -5, -18, -1, 2, 1, 1
};

seq_t a(idx_t n)
{
    /* cache[0] unused; cache[i] holds a(i) for i >= 1 */
    static seq_t *cache   = NULL;
    static idx_t cache_sz = 0;   /* allocated slots (including index 0) */

    if (n < 1) return (seq_t)0;  /* out of range */

    /* grow cache if necessary */
    if (n + 1 > cache_sz) {
        idx_t new_sz = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* allocation failure */
        cache = new_cache;

        /* compute new entries */
        for (idx_t i = (cache_sz == 0 ? 1 : cache_sz); i <= n; ++i) {
            if (i <= 18) {
                cache[i] = init_vals[i-1];
            } else {
                seq_t sum_pos = 0;
                seq_t sum_neg = 0;
                for (int k = 0; k < 18; ++k) {
                    seq_t term = cache[i-1-k];
                    if (coeff[k] >= 0)
                        sum_pos += term * (seq_t)coeff[k];
                    else
                        sum_neg += term * (seq_t)(-coeff[k]);
                }
                /* final value : sum_pos - sum_neg (positives >= negatives) */
                cache[i] = sum_pos - sum_neg;
            }
        }
        cache_sz = new_sz;
    }

    return cache[n];
}
/* ----------  end function implementation  ---------- */
