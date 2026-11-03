/*
 * A379420  a(n) = n + [n*s/r] + [n*t/r], where r = 5^(1/4); s = 5^(1/2); t =
 *          5^(3/4) and [ ] = floor.
 * https://oeis.org/A379420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + [n*r] + [n*r^2] where r = 5^(1/4) and [ ] denotes
 *            floor
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Convert the index to the sequence integer type */
    seq_t sn = (seq_t)n;               /* n as seq_t */

    /* n² and n⁴ as seq_t */
    seq_t n2 = sn * sn;
    seq_t n4 = n2 * n2;

    /* floor(n*r) = floor( (5*n⁴)^(1/4) )
       Compute it safely by two integer square roots:
           floor( sqrt( floor( sqrt(5*n⁴) ) ) )  */
    seq_t val1  = (seq_t)5 * n4;
    seq_t term1 = floor_sqrt(floor_sqrt(val1));

    /* floor(n*r²) = floor( n*sqrt(5) )
       Equivalent to floor( sqrt(5*n²) ) */
    seq_t val2  = (seq_t)5 * n2;
    seq_t term2 = floor_sqrt(val2);

    return sn + term1 + term2;
}
