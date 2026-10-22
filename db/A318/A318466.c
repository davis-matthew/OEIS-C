/*
 * A318466  a(n) = 2*n OR A000203(n), where OR is bitwise-or (A003986) and
 *          A000203 = sum of divisors.
 * https://oeis.org/A318466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = 2*n OR sigma(n)
 * Range:     u8 n <= 107, u16 n <= 9465, u32 n <= 9465, u64 n <= 9465,
 *            u128 n <= 9465, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t s = sigma_of(n);       /* sum of divisors of n */
    seq_t base = (seq_t)n << 1;  /* 2*n as seq_t */
    return base | s;             /* bitwise OR with sigma(n) */
}
