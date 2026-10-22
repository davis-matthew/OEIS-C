/*
 * A352620  Irregular triangle read by rows which are rows of successive n X n
 *          matrices M(n) with entries M(n)[i,j] = i*j mod n+1.
 * https://oeis.org/A352620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = ((n-1 - T_{k-1}) / k + 1) * ((n-1 - T_{k-1}) % k + 1) mod
 *            (k+1) where k is the smallest integer with T_k = k(k+1)(2k+1)/6
 *            >= n and T_0 = 0
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline __int128 sum_sq(idx_t k)
{
    __int128 kk = k;
    return kk * (kk + 1) * (2*kk + 1) / 6;
}

seq_t a(idx_t n)
{
    /* Find k such that T_{k-1} < n <= T_k */
    idx_t low = 1, high = 1;
    while (sum_sq(high) < n) {
        if (high > UINT64_MAX/2) { high = UINT64_MAX; break; }
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (sum_sq(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t k = low;

    /* Compute offset inside M_k (0‑based) */
    idx_t prev_sum = (idx_t)sum_sq(k-1);          /* T_{k-1} fits in idx_t */
    idx_t offset   = n - prev_sum - 1;            /* 0 … k*k-1 */

    idx_t row = offset / k;      /* 0‑based row index */
    idx_t col = offset % k;      /* 0‑based column index */

    /* Convert to 1‑based indices */
    idx_t i = row + 1;
    idx_t j = col + 1;
    idx_t mod = k + 1;

    /* Compute (i*j) mod (k+1) using 128‑bit arithmetic to avoid overflow */
    __int128 prod = (__int128)i * j % mod;

    return (seq_t)prod;
}
