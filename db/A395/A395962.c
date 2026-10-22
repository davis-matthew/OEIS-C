/*
 * A395962  Numbers k such that A395961(k) = 0.
 * https://oeis.org/A395962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*(n-1) + (parity of bits at indices 0,4,8,... of
 *            2*(n-1))
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   parity_bits_4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/parity_bits_4.h"

seq_t a(idx_t n)
{
    seq_t m = ((seq_t)(n - 1)) << 1;          /* m = 2*(n-1)            */
    unsigned p = parity_bits_4(m >> 4);       /* parity of bits 0,4,8,… */
    return m + p;                             /* a(n) as required       */
}
