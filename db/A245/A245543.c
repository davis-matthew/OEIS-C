/*
 * A245543  First differences of A160239.
 * https://oeis.org/A245543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A160239(n+1) - A160239(n), where A160239(0)=1,
 *            A160239(2t)=A160239(t), A160239(4t+1)=8*A160239(t),
 *            A160239(4t+3)=2*A160239(2t+1)+8*A160239(t)
 * Range:     i8 n <= 9, i16 n <= 189, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9285 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A160239_a(idx_t n)
{
    if (n == 0) return 1;
    if ((n & 1) == 0) return A160239_a(n >> 1);
    if ((n & 3) == 1) return 8 * A160239_a((n - 1) >> 2);
    return 2 * A160239_a((n - 1) >> 1) + 8 * A160239_a((n - 3) >> 2);
}

seq_t a(idx_t n)
{
    return A160239_a(n + 1) - A160239_a(n);
}
