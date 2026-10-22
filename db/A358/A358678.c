/*
 * A358678  a(n) = 1 if n is odd and sigma(n) == 2 mod 4, otherwise 0.
 * https://oeis.org/A358678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is odd and σ(n) == 2 (mod 4), otherwise 0
 * Range:     u8 n <= 8776, u16 n <= 8776, u32 n <= 8776, u64 n <= 8776,
 *            u128 n <= 8776, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; n = 0 is out of range */
    if ((n & 1U) == 0) return 0;     /* n must be odd */

    seq_t s = sigma_of(n);           /* σ(n) – sum of all positive divisors */
    return ((s & (seq_t)3) == (seq_t)2) ? (seq_t)1 : (seq_t)0;
}
