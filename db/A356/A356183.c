/*
 * A356183  a(n) = A054406(A001952(n)).
 * https://oeis.org/A356183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A054406(A001952(n)), where A001952(n) = 2n +
 *            floor(sqrt(2n^2)) and A054406(m) = floor((3m +
 *            floor(sqrt(3m^2)))/2)
 * Range:     u8 n <= 2, u16 n <= 43, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-10)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"

static seq_t A001952_a(idx_t n)
{
    seq_t ns = (seq_t)n;
    return 2 * ns + isqrt_seq(2 * ns * ns);
}

static seq_t A054406_a(seq_t m)
{
    seq_t s = isqrt_seq(3 * m * m);
    seq_t k = 3 * m + s;
    return k / 2;
}

seq_t a(idx_t n)
{
    return A054406_a(A001952_a(n));
}
