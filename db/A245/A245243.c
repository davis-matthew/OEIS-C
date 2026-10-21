/*
 * A245243  Triangle, read by rows, defined by T(n,k) = C(n^2 - k^2, n*k -
 *          k^2), for k=0..n, n>=0.
 * https://oeis.org/A245243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(r^2 - k^2, r*k - k^2) * where r =
 *            floor((sqrt(8n+1)-1)/2) * and k = n - r*(r+1)/2
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 29, u64 n <= 57, u128 n <= 108,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Integer square root for idx_t (floor of sqrt). */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = (idx_t)1 << 32;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            low = mid + 1;
        else
        {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* row index */
    idx_t r = (isqrt_u64(8 * n + 1) - 1) / 2;

    /* number of terms before row r */
    idx_t prev = r * (r + 1) / 2;

    /* column index within the row */
    idx_t k = n - prev;

    /* parameters for the binomial coefficient */
    idx_t N = r * r - k * k;
    idx_t K = r * k - k * k;

    return binomial(N, K);
}
