/*
 * A248517  Number of odd divisors > 1 in the numbers 1 through n, counted
 *          with multiplicity.
 * https://oeis.org/A248517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{odd d=3..n} floor(n/d)
 * Range:     u8 n <= 134, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the same as the number of odd divisors d>1 that appear
   as divisors of integers 1..n, each counted with multiplicity. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* Iterate over odd divisors starting from 3 up to n. */
    for (idx_t d = 3; d <= n; d += 2) {
        sum += (seq_t)(n / d);        /* floor division */
    }
    return sum;
}
