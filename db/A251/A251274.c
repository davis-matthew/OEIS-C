/*
 * A251274  Number of (7+1)X(n+1) 0..1 arrays with no 2X2 subblock having
 *          x11-x00 less than x10-x01.
 * https://oeis.org/A251274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 8×(n+1) 0/1 arrays with no 2×2 subblock having
 *            (x11−x00) < (x10−x01). Empirical linear recurrence of order 16
 *            (valid for all n ≥ 1): a(n) = 17·a(n-1) − 135·a(n-2) +
 *            665·a(n-3) − 2275·a(n-4) + … initial values a(1)…a(16) are
 *            listed below
 * Range:     u8 none, u16 n <= 1, u32 n <= 8, u64 n <= 38, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const int COEFF[16] = {
    17, -135, 665, -2275, 5733, -11011, 16445, -19305,
    17875, -13013, 7371, -3185, 1015, -225, 31, -2
};

static const seq_t INIT[16] = {
    10179ULL, 161906ULL, 1501739ULL, 9772880ULL,
    49523566ULL, 208452452ULL, 760734085ULL, 2480493084ULL,
    7386931919ULL, 20428661228ULL, 53150645617ULL,
    131458741458ULL, 311732368552ULL, 713787172312ULL,
    1587653508305ULL, 3447998356240ULL
};

/* Cached sequence values up to the largest n computed so far. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;          /* allocated length (indices 0 .. cache_len-1) */
static idx_t last_computed = 0;     /* largest n for which cache[n] is valid */

seq_t a(idx_t n)
{
    if (n <= 16) return INIT[n - 1];

    /* Ensure cache has space for indices up to n */
    if (cache == NULL) {
        /* Allocate initial buffer large enough for the first query */
        cache_len = (n >= 17 ? n + 2 : 18);
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();
        /* Initialize first 16 terms */
        for (idx_t i = 1; i <= 16; ++i) cache[i] = INIT[i - 1];
        last_computed = 16;
    } else if (n + 1 > cache_len) {
        size_t new_len = (size_t)n + 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute missing terms up to n */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t pos = 0;
            seq_t neg = 0;
            for (int j = 0; j < 16; ++j) {
                seq_t prod = cache[i - (idx_t)(j + 1)] *
                             (seq_t)(unsigned long long) (unsigned long)abs(COEFF[j]);
                if (COEFF[j] >= 0)
                    pos += prod;
                else
                    neg += prod;
            }
            cache[i] = pos - neg;
        }
        last_computed = n;
    }

    return cache[n];
}
