/*
 * A247092  Limiting sequence obtained by taking the sequence of Mersenne
 *          numbers 2^n-1, n=1,2,...(A000225) and applying an infinite process
 *          which is described in the comments.
 * https://oeis.org/A247092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2^(n(n+1)/2 - x^2) where x is the (unique) integer
 *            satisfying x^2 ∈ [(n-1)n/2 + 1, n(n+1)/2]; a(n) = 0 if no such x
 *            exists
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 32, u64 n <= 73, u128 n <= 131,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static inline idx_t isqrt_idx_t(idx_t x)
{
    idx_t lo = 0, hi = x, ans = 0;
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid != 0 && mid > x / mid)          /* mid*mid would overflow / be >x */
            hi = mid - 1;
        else
        {
            ans = mid;
            lo = mid + 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t n_minus_1 = n - 1;
    idx_t B = n * (n + 1) / 2;           /* n(n+1)/2   */
    idx_t A = n_minus_1 * n / 2 + 1;      /* (n-1)n/2 +1 */

    idx_t sqrtB = isqrt_idx_t(B);
    idx_t sq = sqrtB * sqrtB;

    if (sq >= A)
    {
        idx_t exponent = B - sq;
        return pow2(exponent);
    }
    else
        return 0;
}
