/*
 * A311714  Coordination sequence Gal.6.435.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) -3*a(n-2) +4*a(n-3) -5*a(n-4) +6*a(n-5)
 *            -7*a(n-6) *
 *            +8*a(n-7)-9*a(n-8)+10*a(n-9)-10*a(n-10)+10*a(n-11)-10*a(n-12) *
 *            +10*a(n-13)-9*a(n-14)+8*a(n-15)-7*a(n-16)+6*a(n-17)-5*a(n-18) *
 *            +4*a(n-19)-3*a(n-20)+2*a(n-21)-a(n-22) for n > 22. * Initial
 *            values (n = 0..22): * a(0)=1, a(1)=4, a(...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc, realloc */

static seq_t *cache = NULL;   /* buffer holding computed terms */
static size_t cache_len = 0;  /* number of terms stored (cache_len == n+1) */

static const seq_t initial[23] = {
    1, 4, 8, 13, 16, 21, 26, 29, 33, 37, 41, 44, 48, 53, 57, 62,
    67, 71, 75, 79, 83, 86, 89
};

/* coefficients for the recurrence, from a(n-1) to a(n-22) */
static const int coeff[22] = {
    2, -3, 4, -5, 6, -7, 8, -9, 10, -10, 10, -10, 10,
    -9, 8, -7, 6, -5, 4, -3, 2, -1
};

seq_t a(idx_t n) {
    /* expand cache if necessary */
    if (cache == NULL) {
        /* lazily allocate the initial part */
        cache_len = 23;                         /* indices 0..22 available */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) { /* allocation failure, return 0 */
            return (seq_t)0;
        }
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = initial[i];
    }

    /* if requested index already in cache, return it */
    if ((size_t)n < cache_len) {
        return cache[n];
    }

    /* extend cache up to n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL) {
        /* allocation failure */
        return (seq_t)0;
    }
    cache = tmp;

    /* compute new terms from cache_len up to n */
    for (size_t i = cache_len; i <= n; ++i) {
        seq_t posSum = (seq_t)0;
        seq_t negSum = (seq_t)0;
        for (int j = 0; j < 22; ++j) {
            seq_t val = cache[i - 1 - j];
            if (coeff[j] > 0) {
                posSum += val * (seq_t)(coeff[j]);
            } else {
                negSum += val * (seq_t)(-coeff[j]);
            }
        }
        cache[i] = posSum - negSum;   /* posSum >= negSum holds for this sequence */
    }
    cache_len = new_len;

    return cache[n];
}
