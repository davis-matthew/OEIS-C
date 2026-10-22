/*
 * A389539  Maximum number of edges blocked from infinity by n vertices of a
 *          hexagonal grid.
 * https://oeis.org/A389539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = max(0, 2*n - 3 + 3*floor(((n - 3)^2 + 3)/12))
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use signed 64‑bit intermediate values; safe for n up to at least 10⁶. */
    int64_t nn = (int64_t)n;        /* original n as signed */
    int64_t t  = nn - 3;            /* n - 3 ; may be negative for n<3 */
    int64_t v  = (t * t + 3) / 12;  /* floor(((n-3)^2 + 3)/12) */
    int64_t val = 2 * nn - 3 + 3 * v; /* 2*n - 3 + 3*v */
    if (val < 0) val = 0;           /* take max with 0 */
    return (seq_t)val;
}
