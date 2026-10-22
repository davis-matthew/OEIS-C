/*
 * A321490  Triangular table T[n,k] = (n+k)(n^2+k^2), 1 <= k <= n = 1, 2, 3,
 *          ...; read by rows.
 * https://oeis.org/A321490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   A321490 – triangular table T[n,k] = (n+k)(n² + k²), read by
 *            rows. a(n) = (r+k) * (r² + k²) where r = smallest integer with
 *            r(r+1)/2 ≥ n, and k = n – r(r–1)/2
 * Range:     u8 n <= 9, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the row r such that r(r+1)/2 ≥ n.  Uses binary search on 64‑bit
   numbers, performing multiplication in 128‑bit to avoid overflow. */
static inline idx_t _row_of(idx_t n) {
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* safety for non‑positive indices */

    /* Determine (row, column) (r, k) in the triangular table. */
    idx_t r = _row_of(n);
    __uint128_t prev = (__uint128_t)r * (r - 1) / 2;
    idx_t k = (idx_t)(n - prev);

    /* Compute (r+k)*(r² + k²) using seq_t arithmetic. */
    seq_t sum_rk = (seq_t)r + (seq_t)k;          /* (r+k) in seq_t */
    seq_t r_sq   = (seq_t)r * r;                 /* r²   in seq_t */
    seq_t k_sq   = (seq_t)k * k;                 /* k²   in seq_t */
    seq_t inner  = r_sq + k_sq;                  /* r² + k² */
    return sum_rk * inner;                      /* final value */
}
