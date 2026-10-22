/*
 * A392483  Triangle read by rows, with T(n,k) giving the number of n X n
 *          Boolean matrices with exactly k ones having at least one zero row
 *          and at least one zero column.
 * https://oeis.org/A392483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where T(r,k) = Σ_{i=1..r} Σ_{j=1..r} (-1)^{i+j} ·
 *            C(r,i) · C(r,j) · C((r-i)(r-j), k) (r,k) are the row/column
 *            indices of the flattened sequence term n
 * Range:     u8 n <= 20, u16 n <= 40, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Locate the row r and column k corresponding to the 1‑based index n. */
    idx_t cumulative = 0;
    idx_t r;
    for (r = 1; ; ++r)
    {
        idx_t row_len = r * r + 1;          /* entries 0..r² inclusive */
        if (cumulative + row_len >= n)
            break;
        cumulative += row_len;
    }
    idx_t k = n - cumulative - 1;           /* 0‑based column within the row */

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t i = 1; i <= r; ++i)
    {
        seq_t bin_i = binomial(r, i);
        for (idx_t j = 1; j <= r; ++j)
        {
            seq_t bin_j = binomial(r, j);

            idx_t cells = (r - i) * (r - j);
            if (k > cells)
                continue;

            seq_t term = bin_i;
            term *= bin_j;
            term *= binomial(cells, k);

            if (((i + j) & 1) != 0)      /* odd (i+j) → negative contribution */
                sum_neg += term;
            else
                sum_pos += term;
        }
    }

    return sum_pos - sum_neg;
}
