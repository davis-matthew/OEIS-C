/*
 * A262874  Expansion of (Sum_{i>=0} x^(i^2)) * (Sum_{j>=0} x^(j^3)) *
 *          (Sum_{k>=0} x^(k^4)) / (1-x).
 * https://oeis.org/A262874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑negative solutions to x + y^2 + z^3 + u^4 =
 *            n
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = 0xffffffffULL;
    if (hi > n) hi = n;
    while (lo <= hi)
    {
        idx_t mid = (lo + hi) >> 1;
        idx_t sq  = mid * mid;          /* mid <= 2^32-1, sq fits in 64 bits */
        if (sq == n) return mid;
        if (sq <  n)  lo = mid + 1;
        else          hi = mid - 1;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* maximum fourth root of n */
    idx_t u_limit = isqrt_u64( isqrt_u64(n) );   /* floor(n^(1/4)) */

    for (idx_t u = 0; u <= u_limit; ++u)
    {
        idx_t u2 = u * u;
        idx_t u4 = u2 * u2;                     /* u^4 <= n */

        idx_t rem_after_u = n - u4;

        for (idx_t z = 0; ; ++z)
        {
            idx_t z2 = z * z;
            idx_t z3 = z2 * z;                   /* z^3 <= rem_after_u */

            if (z3 > rem_after_u) break;

            idx_t rem = rem_after_u - z3;       /* value for y^2 + x == rem */

            /* number of y with y^2 <= rem is floor(sqrt(rem)) + 1 */
            idx_t ymax = isqrt_u64(rem);
            count += (seq_t)(ymax + 1);
        }
    }

    return count;
}
