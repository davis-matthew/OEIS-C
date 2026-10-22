/*
 * A337102  Number of partitions of n into two positive integer parts (s,t),
 *          s<=t, such that the harmonic mean of the smallest and largest part
 *          is not an integer.
 * https://oeis.org/A337102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..⌊n/2⌋} (ceil(2*i*(n-i)/n) -
 *            floor(2*i*(n-i)/n)), i.e. a(n) = ⌊n/2⌋ minus the number of i in
 *            1..⌊n/2⌋ for which n divides 2*i*(n-i)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle offset: n >= 1.  For n < 1 the function is not intended to be used. */
    if (n < 1)
        return 0;

    idx_t m = n / 2;                 /* ⌊n/2⌋ */
    idx_t divisible = 0;             /* count of i with n | 2*i*(n-i) */

    for (idx_t i = 1; i <= m; ++i) {
        /* Compute 2*i*(n-i) using idx_t to avoid overflow of tiny seq_t types. */
        idx_t prod = 2 * i * (n - i);
        if (prod % n == 0)          /* divisible, then harmonic mean is integer */
            ++divisible;
    }

    /* a(n) = ⌊n/2⌋ - divisible */
    return (seq_t)(m - divisible);
}
