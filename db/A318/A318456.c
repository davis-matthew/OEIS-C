/*
 * A318456  a(n) = n OR A001065(n), where OR is bitwise-or (A003986) and
 *          A001065 = sum of proper divisors.
 * https://oeis.org/A318456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n OR A001065(n) (A001065 = sum of proper divisors)
 * Range:     u8 n <= 143, u16 n <= 9767, u32 n <= 9767, u64 n <= 9767,
 *            u128 n <= 9767, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t num   = (seq_t)n;      /* n as seq_t */
    seq_t sigma = sigma_of(n);   /* sum of all divisors of n */
    seq_t proper = sigma - num;  /* sum of proper divisors */
    return (num | proper);       /* bitwise OR of n and its proper divisor sum */
}
