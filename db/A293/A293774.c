/*
 * A293774  Number of permutations of length n sortable by 3 passes through a
 *          pop-stack.
 * https://oeis.org/A293774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence derived from the rational
 *            generating function G(x) = (2x^10 + 4x^9 + 2x^8 + 5x^7 + 11x^6 +
 *            8x^5 + 6x^4 + 6x^3 + 2x^2 + x - 1) / (4x^10 + 8x^9 + 4x^8 +
 *            10x^7 + 22x^6 + 16x^5 + 8x^4 + 6x^3 + 2x^2 + 2x - 1) The
 *            denominator yields the homogeneous recurrence a(n) =...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq_array.h"

static seq_t *cache = NULL;          /* stored sequence values   */
static size_t cache_size = 0;        /* size of allocated array  */
static size_t computed = 0;          /* number of elements computed (indices 0..computed-1) */

seq_t a(idx_t n)
{
    /* coefficients for the recurrence (indices 1..10 correspond to a[n-1]..a[n-10]) */
    static const unsigned int coeff[10] = {
        2, 2, 6, 8, 16, 22, 10, 4, 8, 4
    };

    /* Initialise cache and first 11 values (a[0]..a[10]) on first call */
    if (cache == NULL) {
        /* Allocate room for indices 0..10 inclusive */
        grow_seq_array(&cache, &cache_size, 11);

        /* Explicit initial values a[0]..a[9] */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)24;
        cache[5] = (seq_t)88;
        cache[6] = (seq_t)303;
        cache[7] = (seq_t)1033;
        cache[8] = (seq_t)3544;
        cache[9] = (seq_t)12220;

        /* Compute a[10] using the forced term 2 from the numerator */
        seq_t sum = 0;
        for (int j = 0; j < 10; ++j) {
            sum += coeff[j] * cache[9 - j];   /* cache[10-(j+1)] = cache[9-j] */
        }
        cache[10] = sum - (seq_t)2;         /* -2 from the numerator */

        computed = 11;    /* values 0 through 10 have been computed */
    }

    /* If requested n is already computed, return it */
    if ((idx_t)computed > n) {
        return cache[n];
    }

    /* Ensure the array is large enough for indices up to n */
    if ((size_t)(n + 1) > cache_size) {
        grow_seq_array(&cache, &cache_size, (size_t)(n + 1));
    }

    /* Compute values from the first missing index up to n */
    for (idx_t k = (idx_t)computed; k <= n; ++k) {
        seq_t sum = 0;
        for (int j = 0; j < 10; ++j) {
            sum += coeff[j] * cache[k - 1 - j];
        }
        cache[k] = sum;
    }

    computed = (size_t)(n + 1);
    return cache[n];
}
