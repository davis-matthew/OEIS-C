/*
 * A335322  Triangle read by rows: T(n, k) = binomial(n, floor((n+k+1)/2))
 *          with k <= n.
 * https://oeis.org/A335322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r, floor((r + k + 1)/2)) where * r =
 *            ceil((sqrt(8n + 1) - 1) / 2) and k = n - (r-1)r/2
 * Range:     u8 n <= 36, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= (unsigned __int128)n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1, return 0 */

    /* Find r = ceil((sqrt(8n + 1) - 1) / 2). */
    idx_t sq = isqrt_u64((unsigned __int128)8 * n + 1);
    idx_t r = (sq - 1) >> 1;
    unsigned __int128 tri = (unsigned __int128)r * (r + 1) / 2;
    if (tri < n) ++r;

    /* Compute k = n - (r-1)r/2. */
    unsigned __int128 prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev_tri;

    /* Compute floor((r + k + 1) / 2). */
    idx_t mid = (r + k + 1) >> 1;

    return binomial(r, mid);
}
