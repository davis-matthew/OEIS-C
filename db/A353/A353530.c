/*
 * A353530  Numbers k such that the smallest prime that does not divide them
 *          is of the form 4m+1.
 * https://oeis.org/A353530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6 * ( n + floor((n-1)/4) ) // n >= 1, offset = 1
 * Range:     u8 n <= 34, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For indices below the offset, the value is irrelevant */
    if (n == 0) return (seq_t)0;

    /* Compute the count of excluded multiples of 5 before n */
    idx_t skip = (n - 1) / 4;          /* floor((n-1)/4) */

    /* Compute the n-th allowed multiplier m */
    seq_t m = (seq_t)n + (seq_t)skip;

    /* The sequence elements are 6 * m */
    return (seq_t)6 * m;
}
