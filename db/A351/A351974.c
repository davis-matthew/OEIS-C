/*
 * A351974  a(n) is the first maximum reached by iterating the reduced Collatz
 *          function R on 4n-1: a(n) = R^s(4n-1), where R(k) = A139391(k) and
 *          s the number of iterations required.
 * https://oeis.org/A351974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * 3^s * (2m+1) - 1, where s = (number of trailing zero
 *            bits of n) + 1, and m = (n - 2^(s-1)) / 2^s. This follows from
 *            the OEIS identities for A351974
 * Range:     u8 n <= 15, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   countTrailingZeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/countTrailingZeros.h"

seq_t a(idx_t n)
{
    /* n must be >= 1 (offset 1). */
    unsigned int tz = countTrailingZeros(n);
    unsigned int s  = tz + 1;                 /* s >= 1          */

    /* Compute 2^(s-1) safely.  For s==64, the shift amount is 63.  */
    idx_t shift1 = ((idx_t)1 << (s - 1));

    /* Determine m.  For s==64, the shift by 64 is undefined; here m is 0. */
    seq_t m_seq;
    if (s == 64)
        m_seq = 0;
    else
        m_seq = (seq_t)((n - shift1) >> s);   /* equivalent to division by 2^s */

    /* Compute 3^s by repeated multiplication.  The loop is at most 64
       iterations, which is trivial. */
    seq_t pow3 = 1;
    for (unsigned int i = 0; i < s; ++i)
        pow3 *= 3;

    /* Compute the final value: 2 * 3^s * (2m+1) - 1. */
    seq_t two_m_plus1 = m_seq * 2 + 1;
    seq_t result = 2 * pow3 * two_m_plus1 - 1;

    return result;
}
