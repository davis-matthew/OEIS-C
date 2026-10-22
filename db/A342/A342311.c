/*
 * A342311  T(n, k) = (n - k + 2)*binomial(2*n, n + k - 2). Triangle read by
 *          rows, T(n, k) for 0 <= k <= n.
 * https://oeis.org/A342311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) * where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2, * and T(r,k) = (r - k + 2) * binomial(2*r, r + k - 2)
 * Range:     u8 n <= 10, u16 n <= 35, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Compute the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2
   using a binary search (no reliance on a sqrt routine).   */
static inline idx_t triangle_row(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;

    /* Expand upper bound until it surpasses n. */
    while (((unsigned __int128)high * (high + 1)) / 2 <= n)
        high <<= 1;

    /* Binary search in [low, high). */
    while (high - low > 1) {
        idx_t mid = (low + high) >> 1;
        if (((unsigned __int128)mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = triangle_row(n);                                   /* row index   */
    idx_t k = n - ((unsigned __int128)r * (r + 1)) / 2;          /* column index */

    /* For r + k < 2 the binomial coefficient is zero. */
    if (r + k < 2)
        return 0;

    idx_t two_r       = r << 1;          /* 2 * r */
    idx_t bin_idx     = r + k - 2;       /* n + k - 2 in the formula */

    seq_t bin         = binomial(two_r, bin_idx);
    seq_t factor      = (seq_t)(r - k + 2);
    return factor * bin;
}
