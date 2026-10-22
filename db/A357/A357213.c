/*
 * A357213  Triangular array read by rows: T(n, k) = number of subsets s of
 *          {1, 2, ..., n} such max(s) - min(s) = k, for n >= 1, 0 <= k <=
 *          n-1.
 * https://oeis.org/A357213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = min R s.t. R*(R+1)/2 ≥ n (row index) k =
 *            n - R*(R-1)/2 - 1 (column index, 0‑based) T(R,0) = R T(R,k) =
 *            (R-k) * 2^(k-1) for k ≥ 1 (number of subsets of {1..R} with
 *            max(s)-min(s)=k)
 * Range:     u8 n <= 53, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static inline idx_t find_row(idx_t n)
{
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 cum = (unsigned __int128)mid * (mid + 1) / 2;
        if (cum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 as safe value */

    idx_t r = find_row(n);

    unsigned __int128 prev_cum = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev_cum - 1;   /* 0‑based column index */

    if (k == 0) {
        return (seq_t)r;
    } else {
        seq_t pow_term = pow2(k - 1);                     /* 2^(k-1) */
        return (seq_t)(r - k) * pow_term;
    }
}
