/*
 * A348958  Triangular array read by rows. T(n,k) =
 *          A002884(n)/A002884(n-k)*2^((n-k)(n-k-1)), n>=0, 0<=k<=n.
 * https://oeis.org/A348958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = 2^{n*(n-1)/2 + (n-k)*(n-k-1)/2} * ∏_{j=n-k+1}^{n} (2^j
 *            - 1) a(idx_t n) reads the triangular array T(n,k) by rows
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 23, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq = mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            lo = mid + 1;
        else
        {
            if (mid == 0)
                break;
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Find row r and position k in that row */
    idx_t value8n1 = 8 * n + 1;
    idx_t sqrtv = isqrt_u64(value8n1);
    idx_t r = (sqrtv - 1) >> 1;             /* floor((sqrt(8n+1)-1)/2) */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;
    if (k > r) k = r;                       /* safety for overflow */
    idx_t m = r - k;                        /* n-k in triangle notation */

    /* Exponent for the 2-power factor */
    idx_t exp1 = r * (r - 1) / 2;
    idx_t exp2 = m * (m - 1) / 2;
    idx_t exp = exp1 + exp2;

    seq_t pow2_val = pow2(exp);

    /* Product of Mersenne numbers */
    seq_t product = (seq_t)1;
    for (idx_t j = m + 1; j <= r; ++j)
    {
        seq_t term = pow2(j) - (seq_t)1;
        product *= term;
    }

    return pow2_val * product;
}
