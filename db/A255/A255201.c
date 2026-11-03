/*
 * A255201  Number of prime factors of n^2.
 * https://oeis.org/A255201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A001222(n) (number of prime factors of n^2)
 * Range:     u8 n <= 360, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 112 terms through n = 945 (2026-11-01)
 * Helpers:   bigomega_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bigomega_seq.h"

seq_t a(idx_t n)
{
    seq_t omega = bigomega_seq((seq_t)n);
    return omega << 1;       /* multiply by 2 */
}
