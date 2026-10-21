/*
 * A285425  Square array A(n,k), n>=1, k>=0, read by antidiagonals, where
 *          column k is the expansion of Sum_{j>=1} (2*j - 1)^k*x^(2*j-1)/(1 -
 *          x^(2*j-1)).
 * https://oeis.org/A285425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the sum of k-th powers of odd divisors of the odd part
 *            of the row number n_row, where (n_row,k) are derived from n by
 *            reading the square array A(n,k) with n>=1,k>=0 along
 *            anti‑diagonals
 * Range:     u8 n <= 38, u16 n <= 70, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   pow_seq_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq_local.h"
/* Helper: binary exponentiation for seq_t */

seq_t a(idx_t N) {
    if (N == 0)
        return 0;                     /* offset is 1 */

    /* Find diagonal d such that (d+1)(d+2)/2 >= N */
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 < N)
        ++d;

    /* Position within diagonal */
    idx_t prev_total = d * (d + 1) / 2;
    idx_t pos = N - prev_total - 1;   /* 0‑based */

    /* Map to coordinates (n_row, k_col) */
    idx_t n_row = 1 + pos;            /* row index, n >= 1 */
    idx_t k_col = d - pos;            /* column index, k >= 0 */

    /* Odd part of n_row */
    idx_t m = n_row;
    while ((m & 1u) == 0)
        m >>= 1;

    /* Sum of k_col‑th powers of odd divisors of m */
    seq_t total = 0;
    for (idx_t d1 = 1; d1 <= m / d1; d1 += 2) {
        if (m % d1 == 0) {
            seq_t term = pow_seq_local((seq_t)d1, (int)k_col);
            total += term;
            idx_t q = m / d1;
            if (q != d1 && (q & 1u)) {
                seq_t term2 = pow_seq_local((seq_t)q, (int)k_col);
                total += term2;
            }
        }
    }
    return total;
}
