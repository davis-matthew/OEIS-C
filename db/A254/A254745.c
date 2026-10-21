/*
 * A254745  Chebyshev polynomials of the second kind, U(n,x)^2, evaluated at x
 *          = sqrt(3)/2.
 * https://oeis.org/A254745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1,3,4,3,1,0 repeating every 6 terms
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {(seq_t)1, (seq_t)3, (seq_t)4,
                                     (seq_t)3, (seq_t)1, (seq_t)0};
    return pattern[(size_t)(n % 6)];
}
