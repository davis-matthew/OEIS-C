/*
 * A258216  Number of permutations of {1,2,3,...,n} such that no multiples of
 *          3 are adjacent.
 * https://oeis.org/A258216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = factorial(n-floor(n/3)) * fallfac(n+1-floor(n/3),
 *            floor(n/3))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* m = floor(n/3), k = n - m */
    idx_t m = n / 3;
    idx_t k = n - m;

    seq_t result = 1;

    /* factorial part: k! */
    for (idx_t i = 2; i <= k; ++i)
        result *= (seq_t)i;

    /* falling factorial part: (n+1-m)_(m) */
    seq_t start = (seq_t)(n + 1 - m);
    for (idx_t i = 0; i < m; ++i)
        result *= (start - (seq_t)i);

    return result;
}
