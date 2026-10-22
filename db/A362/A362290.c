/*
 * A362290  a(n) is the number of parts into which the inner region of the
 *          parabola y = x^2 is divided by n squares inscribed in the parabola
 *          as described in the comments.
 * https://oeis.org/A362290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * ( Sum_{k=1..n} floor(sqrt(4*k-3)) ) + 2 - floor( (
 *            sqrt(4*n-3) - 1 ) / 2 ) Each term is computed exactly using
 *            integer arithmetic only
 * Range:     u8 n <= 22, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for an unsigned __int128 value.
   Returns floor(sqrt(v)).  The returned value fits in 64 bits,
   because sqrt(4*UINT64_MAX - 3) < 2^33.  */
static inline idx_t
isqrt_u128 (unsigned __int128 v)
{
    idx_t low  = 0;
    /* upper bound : sqrt(4*UINT64_MAX - 3) < 2^33.  We take 2^34 to be safe. */
    idx_t high = (idx_t)1ULL << 34;
    idx_t ans  = 0;

    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= v)
        {
            ans = mid;
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a (idx_t n)
{
    if (n == 0)
        return 0;          /* offset is 1, but guard against invalid 0 */

    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Compute 4*k-3 safely in 128 bits to avoid overflow for large k. */
        unsigned __int128 val128 = (unsigned __int128)4 * k - 3;
        idx_t r = isqrt_u128 (val128);
        sum += (seq_t)r;
    }

    unsigned __int128 n128 = (unsigned __int128)4 * n - 3;
    idx_t sqrt_val = isqrt_u128 (n128);
    idx_t inner = (sqrt_val - 1) / 2;

    seq_t result = 2 * sum + 2 - (seq_t)inner;
    return result;
}
