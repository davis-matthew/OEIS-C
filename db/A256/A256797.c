/*
 * A256797  Nonpositive part of the minimal alternating squares representation
 *          of n.
 * https://oeis.org/A256797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Connection to OEIS A256797:
   a(n) is the nonpositive part of the minimal alternating squares representation of n.
   For n >= 1,
   let m = ceil(sqrt(n)) and r = m*m - n.
   Then
     a(n) = r + a(r)       for n != 2,
     a(2) = 2
   with a(0) = a(1) = 0.
   This follows from the recurrence R(n) = m² – R(r) and that the nonpositive
   part of R(n) equals the positive part of R(r) = r + a(r). */

#include "../utils/_floor_isqrt_u64.h"

static seq_t A256797_rec(idx_t n)
{
    if (n <= 1)            /* a(0)=a(1)=0 */
        return 0;

    if (n == 2)            /* special fixed‑point case */
        return 2;

    /* compute ceil(sqrt(n)) safely */
    idx_t m = _floor_isqrt_u64(n);
    if (m * m < n)
        m++;

    idx_t r = m * m - n;   /* smaller than n for n>2 */

    return (seq_t)r + A256797_rec(r);
}

seq_t a(idx_t n)
{
    return A256797_rec(n);
}
