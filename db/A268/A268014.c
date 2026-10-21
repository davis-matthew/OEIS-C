/*
 * A268014  Number of n X 2 0..2 arrays with every repeated value in every row
 *          and column greater than or equal to the previous repeated value.
 * https://oeis.org/A268014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 25*a(n-1) -225*a(n-2) +745*a(n-3) +475*a(n-4)
 *            -6275*a(n-5) * -885*a(n-6) +22525*a(n-7) +22525*a(n-8)
 *            -885*a(n-9) *
 *            -6275*a(n-10)+475*a(n-11)+745*a(n-12)-225*a(n-13)+25*a(n-14)-a(n-15)
 *            * Initial values: a(1)=9,a(2)=81,a(3)=729,a(4)=6084,a(5)=49284,
 *            * a(6)=386884,a(7)=2965284,a(8)=22268961...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;   /* stored sequence values 1..len */
static idx_t  cache_len = 0;    /* number of computed terms */
static idx_t  cache_cap = 0;    /* allocated capacity */

/* first 15 terms of the sequence */
static const seq_t base[15] = {
    (seq_t)9ULL,
    (seq_t)81ULL,
    (seq_t)729ULL,
    (seq_t)6084ULL,
    (seq_t)49284ULL,
    (seq_t)386884ULL,
    (seq_t)2965284ULL,
    (seq_t)22268961ULL,
    (seq_t)164378041ULL,
    (seq_t)1195430625ULL,
    (seq_t)8581613769ULL,
    (seq_t)60904316944ULL,
    (seq_t)427872974400ULL,
    (seq_t)2978730810000ULL,
    (seq_t)20567785069584ULL
};

/* coefficients for the recurrence */
static const int coeff[15] = {
    25, -225, 745, 475, -6275, -885,
    22525, 22525, -885, -6275, 475,
    745, -225, 25, -1
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but harmless */

    /* initialise cache with the first 15 terms */
    if (cache_len == 0) {
        cache_cap = 15;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 15; ++i)
            cache[i] = base[i];
        cache_len = 15;
    }

    /* if the desired term is already computed, return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* extend capacity if needed */
    if (n > cache_cap) {
        cache_cap = n;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* compute all missing terms up to n using the recurrence */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        seq_t val = 0;
        /* sum over the last 15 known values */
        for (int i = 0; i < 15; ++i) {
            seq_t prev = cache[k - i - 2];   /* a(k-i-1) */
            if (coeff[i] >= 0) {
                val += (seq_t)coeff[i] * prev;
            } else {
                val -= (seq_t)(-coeff[i]) * prev;
            }
        }
        cache[k - 1] = val;
    }

    cache_len = n;
    return cache[n - 1];
}
