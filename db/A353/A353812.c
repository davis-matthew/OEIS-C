/*
 * A353812  a(n) = 1 if sigma(n) is of the form 4m+2, otherwise 0.
 * https://oeis.org/A353812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9921, u16 n <= 9921, u32 n <= 9921, u64 n <= 9921,
 *            u128 n <= 9921, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 if σ(n) ≡ 2 (mod 4), otherwise 0. */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* out of defined range */
    seq_t s = sigma_of(n);        /* compute the divisor sum σ(n) */
    return (s % (seq_t)4 == (seq_t)2) ? (seq_t)1 : (seq_t)0;
}
