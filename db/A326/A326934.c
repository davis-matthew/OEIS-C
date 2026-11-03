/*
 * A326934  Table of A(n,k) read by antidiagonals, where A(n,k) = (n*k) mod
 *          (n+1).
 * https://oeis.org/A326934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (i * j) mod (i + 1) where (i, j) are the coordinates of
 *            the n-th entry read by antidiagonals in the infinite matrix
 *            A(n,k) = (n*k) mod (n+1)
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Validated: standard, pass, 102 terms through n = 1220 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the diagonal number d such that T(d-1) < n <= T(d),
   where T(d) = d*(d+1)/2.  Uses binary search on 64‑bit integers. */
static inline idx_t _diag_for_idx(idx_t n) {
    idx_t low = 1, high = 1;
    /* Exponentially increase high until T(high) >= n */
    while ((unsigned __int128)high * (high + 1) / 2 < n) {
        high <<= 1;
    }
    /* Binary search between low and high-1 */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return low;
}

seq_t a(idx_t n) {
    /* Determine diagonal and position within it */
    idx_t d   = _diag_for_idx(n);                 /* diagonal number */
    idx_t prev = (d - 1) * d / 2;                 /* entries before this diagonal */
    idx_t f    = n - prev;                        /* 1‑based position in diagonal */
    idx_t i    = f;                               /* row index (n in A(n,k))   */
    idx_t j    = d - f + 1;                       /* column index (k in A(n,k)) */

    /* Compute (i * j) mod (i + 1) safely using 128‑bit arithmetic */
    idx_t mod = i + 1;
    unsigned __int128 prod = (unsigned __int128)i * (unsigned __int128)(j % mod);
    seq_t result = (seq_t)(prod % mod);
    return result;
}
