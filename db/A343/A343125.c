/*
 * A343125  Triangle T(k, n) = (n+3)*(k-n) - 4, k >= 2, 1 <= n <= k-1, read by
 *          rows.
 * https://oeis.org/A343125
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(k,n) where the triangle T(k,n) = (n+3)*(k-n)-4 * is
 *            read row‑by‑row. Row k has indices n = 1..k-1, and the *
 *            cumulative number of terms up to row k is k(k-1)/2. For an *
 *            input index n≥2, find the smallest k with k(k-1)/2 > n-2, * let
 *            t = n-2 - (k-2)(k-1)/2 + 1 (1‑based position in row),...
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;              /* defined only for n ≥ 2 */

    /* Remaining index inside the flattened triangle (0‑based). */
    idx_t rem = n - 2;

    /* Find the row k such that rem < k-1 (row length). */
    idx_t k = 2;
    idx_t row_len = 1;                /* k-1 for k=2 */
    while (rem >= row_len)
    {
        rem -= row_len;
        ++k;
        row_len = k - 1;
    }

    /* Position within the row (1‑based). */
    idx_t pos = rem + 1;

    /* Compute value using seq_t arithmetic. */
    seq_t k_val   = (seq_t)k;
    seq_t pos_val = (seq_t)pos;

    seq_t term = (pos_val + (seq_t)3) * (k_val - pos_val) - (seq_t)4;
    return term;
}
