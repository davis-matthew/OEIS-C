/*
 * A262705  Triangle: Newton expansion of C(n,m)^4, read by rows.
 * https://oeis.org/A262705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T_4 ⌊n_row⌋,m where * T_4(n,m) = ∑_{j=m}^n (-1)^{n-j} *
 *            C(n,j) * C(j,m)^4 * n_row and m are obtained by flattening the
 *            triangle row-wise * (a(0)=T_4(0,0), a(1)=T_4(1,0),
 *            a(2)=T_4(1,1), …)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t k)
{
    /* find row n such that n(n+1)/2 <= k < (n+1)(n+2)/2 */
    idx_t n = 0;
    for (;; ++n) {
        idx_t start = n * (n + 1) / 2;          /* first index of row n */
        idx_t next  = (n + 1) * (n + 2) / 2;    /* first index of row n+1 */
        if (k < next) {
            /* within this row */
            idx_t m = k - start;                /* column within the row */
            /* compute T_4(n,m) */
            seq_t sum_pos = 0, sum_neg = 0;     /* accumulate positive & negative terms */
            for (idx_t j = m; j <= n; ++j) {
                /* binomial coefficient C(j,m) */
                seq_t bin_jm = binomial(j, m);
                /* compute (C(j,m))^4 */
                seq_t power = bin_jm;
                power *= power;   /* ^2 */
                power *= bin_jm;  /* ^3 */
                power *= bin_jm;  /* ^4 */

                /* binomial coefficient C(n,j) */
                seq_t bin_nj = binomial(n, j);

                /* term = C(n,j) * (C(j,m))^4 */
                seq_t term = bin_nj * power;

                /* sign = (-1)^(n-j) */
                if ((n - j) & 1) {
                    sum_neg += term;
                } else {
                    sum_pos += term;
                }
            }
            /* result = sum_pos - sum_neg (guaranteed non‑negative) */
            return sum_pos - sum_neg;
        }
    }
}
