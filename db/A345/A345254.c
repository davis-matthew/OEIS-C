/*
 * A345254  Dispersion of A004754, a rectangular array T(n,k) read by downward
 *          antidiagonals.
 * https://oeis.org/A345254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = value of T(n_index,k_index) read by downward
 *            antidiagonals where T(0,k)=2^(k-1) and
 *            T(n,k)=n+2^(floor(log2(n))+k) for n≥1
 * Range:     u8 n <= 36, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static idx_t floor_log2_uint64(idx_t n) {
    idx_t r = 0;
    while (n >>= 1) ++r;
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* sequence is defined for n≥1, return 0 for safety */

    /* Find diagonal d such that d*(d+1)/2 ≥ n */
    idx_t d = 0;
    while (d * (d + 1) / 2 < n) ++d;

    /* Position t within the diagonal (0‑based) */
    idx_t prev_cumul = (d - 1) * d / 2;
    idx_t t = n - prev_cumul - 1;

    /* Corresponding matrix indices */
    idx_t n_idx = t;          /* row index (n) */
    idx_t k_idx = d - t;      /* column index (k) */

    if (n_idx == 0) {
        /* Top row: T(0,k) = 2^(k-1) */
        return pow2(k_idx - 1);
    } else {
        /* Interior: T(n,k) = n + 2^(floor(log2(n))+k) */
        idx_t e = floor_log2_uint64(n_idx);
        idx_t exp = e + k_idx;
        seq_t val = pow2(exp);
        val += n_idx;               /* add n_idx to the power of two */
        return val;
    }
}
