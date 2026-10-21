/*
 * A249769  Sequence of distinct least positive numbers such that the average
 *          of the first n terms is a factorial.
 * https://oeis.org/A249769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 21, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for A249769
   a(2*n-1) = n!
   a(2*n)   = n! * (2*n*n + 1)
   a(1) = 1, a(2) = 3, ... */

#include <stdint.h> /* for uint64_t if needed */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For safety return 0 for n == 0. */
    if (n == 0)
        return (seq_t)0;

    /* k = ceil(n/2) = (n+1)/2 using integer arithmetic.
       For odd n = 2*k-1 this gives k, for even n = 2*k this gives k.   */
    idx_t k = (n + 1) / 2;

    /* Compute k! iteratively.  All arithmetic is performed in seq_t
       to avoid intermediate overflow on arbitrary-precision builds. */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= k; ++i)
        fact *= (seq_t)i;

    /* If n is even, multiply by (2*k*k + 1). */
    if ((n & 1) == 0) {
        seq_t mult = ((seq_t)2 * (seq_t)k * (seq_t)k) + (seq_t)1;
        fact *= mult;
    }

    return fact;
}
