/*
 * A352988  Matrix inverse of triangle A352650.
 * https://oeis.org/A352988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) is the n‑th element of the first column of the inverse of
 *            the lower‑triangular matrix T defined by T(n,n) = 1, T(n,n-1) =
 *            1-n (n>0), T(n,n-2) = 1-n (n>1), all other entries 0. The
 *            sequence may be generated directly by the piecewise rule: let m
 *            = ⌊(⌊√(8n+1)⌋ – 1)/2⌋ // block index let r = n –...
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for an unsigned __int128 argument.
   Returns ⌊√x⌋ as an idx_t (uint64_t).  */
static inline idx_t isqrt_u128(unsigned __int128 x)
{
    idx_t low = 0;
    idx_t high = (idx_t)-1;          /* 2^64−1 */
    idx_t ans = 0;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
        {
            ans = mid;
            low = mid + 1;
        }
        else
            high = mid - 1;
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Compute floor(sqrt(8*n+1)) using 128‑bit arithmetic to avoid overflow. */
    unsigned __int128 val = (unsigned __int128)n * (unsigned __int128)8 + 1;
    idx_t s = isqrt_u128(val);

    /* Block index m = floor((s-1)/2).  */
    idx_t m = (s - 1) >> 1;   /* (s-1)/2 using shift because div by 2 */

    /* Starting index of this block.  */
    idx_t offset = m * (m + 1) / 2;

    /* Position within the block.  */
    idx_t r = n - offset;

    if (m == 0)
        return (seq_t)1;

    if (m == 1)
        return (r == 1) ? (seq_t)1 : (seq_t)0;

    /* For m >= 2.  */
    if (r < m - 2)
        return (seq_t)0;

    if (r == m - 2 || r == m - 1)
        return -(seq_t)(m - 1);

    /* r == m */
    return (seq_t)1;
}
