/*
 * A281396  Number of nX4 0..1 arrays with no element equal to more than two
 *          of its horizontal, diagonal or antidiagonal neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A281396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 8*a(n-3) + a(n-4) - 17*a(n-5) -
 *            37*a(n-6) - 27*a(n-7) + 20*a(n-8) + 62*a(n-9) + 50*a(n-10) -
 *            14*a(n-11) - 45*a(n-12) - 21*a(n-13) + 9*a(n-14) + 9*a(n-15) for
 *            n > 21, with initial values a(1)=0, a(2)=52, a(3)=314,
 *            a(4)=1020, a(5)=2886, a(6)=7064, a(7)=17194, a(8)=42396,...
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 21, u64 n <= 49, u128 n <= 106,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

/* Helper macro to avoid using signed arithmetic on seq_t */
#define ADD_POS(res, term, mult)                           \
    do {                                                   \
        (res) += mul_small((term), (mult));                \
    } while (0)

static const seq_t init_vals[21] = {
    (seq_t)0ULL,        /* a(1) */
    (seq_t)52ULL,       /* a(2) */
    (seq_t)314ULL,      /* a(3) */
    (seq_t)1020ULL,     /* a(4) */
    (seq_t)2886ULL,     /* a(5) */
    (seq_t)7064ULL,     /* a(6) */
    (seq_t)17194ULL,    /* a(7) */
    (seq_t)42396ULL,    /* a(8) */
    (seq_t)101410ULL,   /* a(9) */
    (seq_t)239524ULL,   /* a(10) */
    (seq_t)564328ULL,   /* a(11) */
    (seq_t)1316460ULL,  /* a(12) */
    (seq_t)3049172ULL,  /* a(13) */
    (seq_t)7035996ULL,  /* a(14) */
    (seq_t)16153984ULL, /* a(15) */
    (seq_t)36917768ULL, /* a(16) */
    (seq_t)84107390ULL, /* a(17) */
    (seq_t)191019984ULL,/* a(18) */
    (seq_t)432482066ULL,/* a(19) */
    (seq_t)976716232ULL,/* a(20) */
    (seq_t)2200907846ULL /* a(21) */
};

/* Recurrence coefficients for a(n) where n>21 */
/* Index 0: coefficient of a(n-1), index 14: coefficient of a(n-15) */
static const int coeffs[15] = {
    1,  2,  8,   1, -17, -37, -27, 20, 62, 50, -14, -45, -21, 9, 9
};

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n == 0) return (seq_t)0;                 /* undefined, but safe */

    /* Initialise cache with first 21 terms */
    if (!cache) {
        cache = (seq_t*)malloc(21 * sizeof(seq_t));
        for (idx_t i = 0; i < 21; ++i)
            cache[i] = init_vals[i];
        cache_len = 21;
    }

    /* Return already computed value if available */
    if (n <= cache_len)
        return cache[n - 1];

    /* Extend cache if necessary */
    if (n > cache_len) {
        seq_t *new_cache = (seq_t*)realloc(cache, n * sizeof(seq_t));
        if (!new_cache) { /* allocation failed; return 0 as fallback */
            return (seq_t)0;
        }
        cache = new_cache;

        /* Compute new terms using the recurrence */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 21) {
                cache[i - 1] = init_vals[i - 1]; /* safety, though already set */
                continue;
            }
            seq_t pos_sum = 0;
            seq_t neg_sum = 0;
            for (int k = 1; k <= 15; ++k) {
                int c = coeffs[k - 1];
                idx_t j = i - k - 1;            /* index in cache of a(i-k) */
                if (c > 0) {
                    ADD_POS(pos_sum, cache[j], (unsigned int)c);
                } else {
                    ADD_POS(neg_sum, cache[j], (unsigned int)(-c));
                }
            }
            cache[i - 1] = pos_sum - neg_sum;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
