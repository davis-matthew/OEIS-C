/*
 * A351584  Triangle read by rows: T(n,k) = A(k,n-k), 1 <= k < n, 2 <= n,
 *          where A(m,n) is the number of distinct strings consisting of one
 *          X, 2*m Y's and 2*n Z's in which the X lies to the right of at
 *          least m Y's and at least n Z's.
 * https://oeis.org/A351584
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,k) = (4*(n-k)*k + 3*n + 2)*C(2n+2, 2k+1)/(4n+4) -
 *            n*C(n,k)^2/2, where a(idx) = T(n,k) read by rows, offset 2
 * Range:     u8 none, u16 n <= 12, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t index) {
    if (index < 2) return 0;   /* behavior below the offset is irrelevant */

    /* Convert linear index to triangle coordinates (row, k). */
    idx_t t = index - 2;           /* zero‑based position in the triangle */
    idx_t row = 2;
    while ((row * (row - 1)) / 2 <= t) {   /* find minimal row with cum > t */
        row++;
    }
    idx_t prev_cum = ((row - 1) * (row - 2)) / 2;
    idx_t offset = t - prev_cum;      /* 0‑based offset within the row */
    idx_t k = offset + 1;             /* k ranges from 1 to row-1 */

    /* Compute the two terms of the formula using integer arithmetic. */
    seq_t term1_num = (seq_t)(4u * (row - k) * k + 3 * row + 2)
                   * binomial((idx_t)(2 * row + 2), (idx_t)(2 * k + 1));

    seq_t binom_nk = binomial(row, k);
    seq_t term2_num = (seq_t)row * binom_nk * binom_nk;

    /* Common denominator: 8n + 8.  Rewrite the difference so that division
       works on integers only. */
    seq_t factor = (seq_t)(4 * row + 4);
    seq_t numerator   = 2 * term1_num - factor * term2_num;
    seq_t denominator = (seq_t)(8 * row + 8);

    return numerator / denominator;
}
