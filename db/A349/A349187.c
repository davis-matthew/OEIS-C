/*
 * A349187  Number of tilings of a 5 X n rectangle using n pentominoes of
 *          shapes X, Y, Z.
 * https://oeis.org/A349187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence * * a(n) + 16*a(n-1) +
 *            5*a(n-2) + 2*a(n-4) + 33*a(n-5) + 38*a(n-6) + 7*a(n-7) * +
 *            8*a(n-8) + 20*a(n-9) + 14*a(n-10) + 10*a(n-11) - a(n-12) -
 *            8*a(n-13) * - 7*a(n-14) - 13*a(n-15) - 3*a(n-16) + a(n-17) = 0 *
 *            * with a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=6,...
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 39, u64 n <= 75, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const int d_coeff[21] = {
    1,   /* d[0] = 1 */
    0,   /* d[1] = 0 */
    0,   /* d[2] = 0 */
    0,   /* d[3] = 0 */
   -3,   /* d[4] = -3 */
   -13,  /* d[5] = -13 */
    -7,  /* d[6] = -7 */
    -8,  /* d[7] = -8 */
    -1,  /* d[8] = -1 */
    10,  /* d[9] = 10 */
    14,  /* d[10] = 14 */
    20,  /* d[11] = 20 */
     8,  /* d[12] = 8 */
     7,  /* d[13] = 7 */
    38,  /* d[14] = 38 */
    33,  /* d[15] = 33 */
     2,  /* d[16] = 2 */
     0,  /* d[17] = 0 */
     5,  /* d[18] = 5 */
    16,  /* d[19] = 16 */
    11   /* d[20] = 11 */
};

static const int n_coeff[21] = {
    1,    /* numerator x^0 */
    0,    /* x^1 */
    0,    /* x^2 */
    0,    /* x^3 */
   -3,    /* x^4 */
   -7,    /* x^5 */
   -1,    /* x^6 */
   -2,    /* x^7 */
    1,    /* x^8 */
    2,    /* x^9 */
    4,    /* x^10 */
    0,    /* x^11 */
    0,    /* x^12 */
    7,    /* x^13 */
   14,    /* x^14 */
    7,    /* x^15 */
    0,    /* x^16 */
    0,    /* x^17 */
    5,    /* x^18 */
    6,    /* x^19 */
    1     /* x^20 */
};

static seq_t *cache = NULL;
static idx_t cache_capacity = 0;
static idx_t cache_size    = 0;

/* Initialise the cache with the first 21 terms (0 … 20). */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    cache_capacity = 21;
    cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
    if (cache == NULL)
        abort();

    /* Compute the first 21 terms from the inhomogeneous equations. */
    for (idx_t m = 0; m < 21; ++m) {
        seq_t sum = 0;
        /* Sum over k = 1 .. m (but at most 20 indices). */
        for (int k = 1; k <= (int)m; ++k) {
            sum += (seq_t)((int)d_coeff[k] * cache[m - k]);
        }
        /* a_m = n_m - sum  (mod 2^w) */
        cache[m] = (seq_t)((seq_t)n_coeff[m] - sum);
    }
    cache_size = 21;
}

/* Ensure that cache contains values up to and including n. */
static void ensure_cache(idx_t n)
{
    init_cache();

    if ((idx_t)n + 1 > cache_capacity) {
        idx_t new_cap = cache_capacity;
        while (new_cap <= (idx_t)n + 1)
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL)
            abort();
        cache = new_cache;
        cache_capacity = new_cap;
    }

    if (cache_size > n)
        return;

    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t term = 0;
        for (int k = 1; k <= 20; ++k) {
            term += (seq_t)((int)d_coeff[k] * cache[i - k]);
        }
        cache[i] = (seq_t)(-term);
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
