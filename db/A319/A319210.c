/*
 * A319210  a(n) = phi(n^2 - 1)/2 where phi is A000010.
 * https://oeis.org/A319210
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(n^2 - 1)/2
 * Range:     u8 n <= 16, u16 n <= 258, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 981 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_seq.h"

seq_t a(idx_t n)
{
    /* n >= 2 */
    seq_t m = (seq_t)n * (seq_t)n - 1;
    seq_t ph = totient_seq(m);
    return ph / 2;
}
