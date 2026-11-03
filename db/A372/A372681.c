/*
 * A372681  a(n) = phi(17 * n)/16.
 * https://oeis.org/A372681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = phi(17*n)/16
 * Range:     u8 n <= 15, u16 n <= 4781, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9716 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t nn = n;
    seq_t m = 17 * nn;
    seq_t ph = totient_seq(m);
    return ph / 16;
}
