/*
 * A347834  An array A of the positive odd numbers, read by antidiagonals
 *          upwards, giving the present triangle T.
 * https://oeis.org/A347834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is obtained by reading the array A(k,n) * where * A(k,0) =
 *            2*(k + ⌊k/3⌋) - 1 (odd numbers ≡ 1,3,7 (mod 8)) * A(k,n) =
 *            4*A(k,n-1) + 1 (n ≥ 1) * * The array is traversed by
 *            antidiagonals upwards: * for each diagonal sum s = k-1 + n, the
 *            pairs (k,n) with * k = s+1, s, … , 1 are visited in that...
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 152, u64 n <= 560,
 *            u128 n <= 2144, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t idx)
{
    if (idx == 0)            /* defined only for n ≥ 1, value for 0 is irrelevant */
        return 0;

    /* Find diagonal s such that the cumulative count up to s is ≥ idx.
     * Number of elements on diagonal s is s+1.
     * Cumulative count for s-1 diagonals: s*(s+1)/2.
     */
    idx_t s = 0;
    while (((s + 1) * (s + 2) / 2) < idx)
        ++s;

    /* Number of elements before diagonal s */
    idx_t prev = s * (s + 1) / 2;

    /* Position within diagonal s (0‑based) */
    idx_t pos = idx - prev - 1;

    /* Coordinates in the array A(k,n) */
    idx_t k     = s - pos + 1;   /* row index   (k ≥ 1) */
    idx_t row_n = pos;           /* column index (n ≥ 0) */

    /* Base value A(k,0) */
    idx_t base = 2 * (k + k / 3) - 1;

    /* Compute A(k,row_n) by repeated application of the recurrence
     * A(k,n) = 4*A(k,n-1)+1.
     */
    seq_t val = (seq_t)base;
    for (idx_t i = 0; i < row_n; ++i)
        val = val * 4 + 1;

    return val;
}
