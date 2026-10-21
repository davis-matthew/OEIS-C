/*
 * A298792  Coordination sequence for the bil tiling (or net) with respect to
 *          a trivalent node of the first type.
 * https://oeis.org/A298792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 3*a(n-5) -
 *            3*a(n-6) + 4*a(n-7) - 4*a(n-8) + 4*a(n-9) - 3*a(n-10) +
 *            3*a(n-11) - 2*a(n-12) + 2*a(n-13) - 2*a(n-14) + a(n-15) -
 *            a(n-16) for n>16; * initial values: a(0)=1, a(1)=3, a(2)=6,
 *            a(3)=9, a(4)=11, a(5)=14, a(6)=19, a(7)=22, a(8)=23, a(9)=25,
 *            *...
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoisation cache */
static seq_t *cache = NULL;      /* pointer to cached values */
static size_t cache_len = 0;     /* first index not yet cached (size of valid data) */
static size_t cache_cap = 0;     /* allocated capacity of cache */

/* Known first 17 values */
static const seq_t init_vals[17] = {
    1ULL, 3ULL, 6ULL, 9ULL, 11ULL, 14ULL, 19ULL, 22ULL, 23ULL, 25ULL,
    30ULL, 35ULL, 35ULL, 36ULL, 41ULL, 46ULL, 49ULL
};

/* Recurrence coefficients c[0] .. c[15] for a(n-1) .. a(n-16) */
static const int coeff[16] = {
    1, -2, 2, -2, 3, -3, 4, -4, 4, -3, 3, -2, 2, -2, 1, -1
};

/* Ensure capacity for index n */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
    if (cache != NULL) {
        for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        delete [] cache;
    }
    cache = new_cache;
#else
    new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
#endif
    cache_cap = new_cap;
}

/* Compute a(n) with memoisation */
seq_t a(idx_t n)
{
    /* Ensure we can store up to index n */
    ensure_capacity(n);

    /* Initialise cache with the 17 known values on first use */
    if (cache_len == 0) {
        for (size_t i = 0; i < 17; ++i) cache[i] = init_vals[i];
        cache_len = 17;
    }

    /* If the value is already cached, return it */
    if ((size_t)n < cache_len) return cache[(size_t)n];

    /* Compute missing values up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* For i <= 16 the cache already contains the correct value */
        /* The recurrence starts at i = 17 */
        seq_t pos = 0;   /* sum of positive coefficient terms */
        seq_t neg = 0;   /* sum of absolute values of negative coefficient terms */

        for (int j = 0; j < 16; ++j) {
            seq_t term = cache[i - 1 - (size_t)j];
            if (coeff[j] > 0)
                pos += (seq_t)coeff[j] * term;
            else
                neg += (seq_t)(-coeff[j]) * term;
        }
        cache[i] = pos - neg;   /* pos ≥ neg for this sequence, so subtraction is safe */
    }

    /* Update length and return the requested term */
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
