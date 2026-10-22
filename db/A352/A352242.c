/*
 * A352242  Regular triangle T(n,k) = (n-k)*(n^3-k^3) for n>=2 and 1 <= k <=
 *          n-1, read by rows.
 * https://oeis.org/A352242
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (r-k)*(r^3-k^3) read by rows; r and k are derived from n
 * Range:     u8 n <= 7, u16 n <= 121, u32 n <= 9280, u64 n <= 9280,
 *            u128 n <= 9280, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Floor square root for 0 ≤ x < 2^64 using binary search */
static inline idx_t floor_isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 m2 = (unsigned __int128)mid * mid;
        if (m2 == (unsigned __int128)x) return mid;
        if (m2 < (unsigned __int128)x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* undefined for the OEIS offset */

    /* t is the zero‑based index into the flattened triangle (t=0 for n=2). */
    idx_t t = n - 2;

    /* Determine the row r: the smallest r with r(r-1)/2 > t */
    idx_t val = (idx_t)((unsigned __int128)8 * t + 1);
    idx_t sqrt_val = floor_isqrt_u64(val);
    idx_t r = (sqrt_val + 1) / 2 + 1;

    /* Number of terms before row r */
    idx_t cprev = (idx_t)((unsigned __int128)(r - 2) * (r - 1) / 2);

    /* Column index k within the row (1‑based) */
    idx_t k = t - cprev + 1;

    /* Compute T(r,k) in seq_t arithmetic */
    seq_t R = (seq_t)r;
    seq_t K = (seq_t)k;
    seq_t R_minus_K = R - K;

    seq_t R_sq = R * R;
    seq_t R_cu = R_sq * R;
    seq_t K_sq = K * K;
    seq_t K_cu = K_sq * K;

    seq_t diff = R_cu - K_cu;
    seq_t result = R_minus_K * diff;
    return result;
}
