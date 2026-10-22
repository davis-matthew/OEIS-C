/*
 * A336483  a(n) = floor(n/10) + (5 times last digit of n).
 * https://oeis.org/A336483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(n/10) + 5*(n mod 10)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute floor(n/10) and 5 times the last digit (n mod 10). */
    return (seq_t)(n / 10) + (seq_t)(5 * (n % 10));
}
