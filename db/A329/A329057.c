/*
 * A329057  1-parking triangle T(r, i, 1) read by rows: T(r, i, k) = (r +
 *          1)^(i-1)*binomial(k*(r + 1) + r - i - 1, r - i) with k = 1 and 0
 *          <= i <= r.
 * https://oeis.org/A329057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is built by flattening the 1‑parking triangle T(r,i,1) read
 *            by rows. For each n≥0 find integers r,i such that T_r ≤ n <
 *            T_{r+1}, where T_r = r(r+1)/2 (terms before row r), i = n - T_r
 *            (0 ≤ i ≤ r). Then if i == 0 : a(n) = Catalan(r) else : a(n) =
 *            (r+1)^(i-1) * binomial(2r - i, r - i)
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 73, u64 n <= 166,
 *            u128 n <= 430, big any n
 * Validated: standard, pass, 71 terms through n = 9058 (2026-11-01)
 * Helpers:   binomial, catalan, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required utilities */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    /* Find r: largest such that r*(r+1)/2 <= n */
    idx_t r = 0;
    /* Simple loop; r is at most about sqrt(2n) */
    while ((r + 1) * (r + 2) / 2 <= n) {
        r++;
    }
    idx_t i = n - r * (r + 1) / 2;

    if (i == 0) {
        return catalan(r);
    } else {
        seq_t pow_val = ipow_idx((seq_t)(r + 1), i - 1);
        seq_t bin_val = binomial((idx_t)(2 * r - i), (idx_t)(r - i));
        return pow_val * bin_val;
    }
}
