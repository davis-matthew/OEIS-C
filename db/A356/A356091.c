/*
 * A356091  a(n) = A001952(A054406(n)).
 * https://oeis.org/A356091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001952(A054406(n)) = floor(m*(2+sqrt(2))) with m =
 *            floor(n*(3+sqrt(3))/2)
 * Range:     u8 n <= 5, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-10)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"

static seq_t A054406_val(seq_t ns)
{
    seq_t s = isqrt_seq(3 * ns * ns);   /* floor(n*sqrt(3)) */
    seq_t k = 3 * ns + s;
    return k / 2;
}

static seq_t A001952_val(seq_t m)
{
    seq_t r = isqrt_seq(2 * m * m);     /* floor(m*sqrt(2)) */
    return 2 * m + r;
}

seq_t a(idx_t n)
{
    seq_t m = A054406_val((seq_t)n);
    return A001952_val(m);
}
