/*
 * A355010  Array read by ascending antidiagonals: T(n, k) is the number of
 *          n-core partitions with k corners.
 * https://oeis.org/A355010
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained from the array T(n,k) read by ascending
 *            antidiagonals. T(n,k) counts n‑core partitions with k corners
 *            and is given by T(n,k) = Σ_{i=1..min(k,⌊n/2⌋)}
 *            (C(k,i)*C(k,i-1)/k) * C(n+2*(k-i), 2k) In the OEIS array the
 *            first index of T is 1, and the values appear in the order (1,1),
 *            (2,1)...
 * Range:     u8 n <= 24, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiplicative binomial: C(n,k) */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* T(n_row, k_col) with n_row >=1, k_col >=1
   Uses the formula where the n in the binomial argument is n_row+1. */
static seq_t T_value(idx_t n_row, idx_t k_col)
{
    idx_t n_form = n_row + 1;                  /* shift as explained above */
    idx_t max_i  = k_col;
    if (max_i > n_form / 2) max_i = n_form / 2;

    seq_t sum = 0;
    for (idx_t i = 1; i <= max_i; ++i)
    {
        seq_t bin1 = binom_idx(k_col, i);
        seq_t bin2 = binom_idx(k_col, i - 1);
        seq_t N    = (bin1 * bin2) / (seq_t)k_col;

        /* binom(n_form + 2*(k-i), 2*k) */
        seq_t bin_mult = binom_idx(n_form + 2 * (k_col - i), 2 * k_col);

        sum += N * bin_mult;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* undefined for n < 2 (offset 2) */

    idx_t m = n - 2;       /* zero‑based index in the antidiagonal order */

    /* find the diagonal length t (smallest t with t*(t+1)/2 > m) */
    idx_t t = 1;
    while ((t * (t + 1)) / 2 <= m) ++t;

    /* position r within this diagonal (0‑based) */
    idx_t r = m - ((t - 1) * t) / 2;

    /* map to (row, column) in the array T */
    idx_t n_row = t - r;   /* row index in T (1‑based) */
    idx_t k_col = r + 1;   /* column index in T (1‑based) */

    return T_value(n_row, k_col);
}
