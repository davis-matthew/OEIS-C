/*
 * A273619  Table read by antidiagonals (n>1, k>0): A(n,k) = leading digit of
 *          k in base n.
 * https://oeis.org/A273619
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  tabl, base, easy, nonn
 * Formula:   a(n) = leading digit of k in base n, where (n,k) is the *
 *            position in the array A(n,k) read by antidiagonals. The array *
 *            indexes start at n>1, k>0 and the sequence is offset 2. * The
 *            mapping is: * p = n-2 (0‑based sequence position) * d =
 *            floor((sqrt(8p+1)-1)/2) (diagonal index) * left = d*(d+1)...
 * Range:     u8 n <= 8595, u16 n <= 8595, u32 n <= 8595, u64 n <= 8595,
 *            u128 n <= 8595, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t m)
{
    if (m < 2) return 0;          /* undefined for offsets below 2 */

    /* 1‑based position offset: p = m-2   */
    idx_t p = m - 2;

    /* Find diagonal d such that d*(d+1)/2 <= p < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while (d * (d + 1) / 2 <= p) ++d;
    --d;                           /* adjust to largest satisfying left <= p */

    idx_t left = d * (d + 1) / 2;
    idx_t k     = p - left;         /* 0‑based index on diagonal */
    idx_t n_val = d + 2 - k;        /* row index in the array */
    idx_t kcol  = d + 3 - n_val;    /* column (k) in the array */

    /* Compute leading digit of kcol in base n_val. */
    idx_t val = kcol;
    while (val >= n_val) val /= n_val;
    return (seq_t)val;
}
