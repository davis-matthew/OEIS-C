/*
 * A382532  a(n) = Product_{k=1..n} (n*k-k+1).
 * https://oeis.org/A382532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (n*k-k+1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t term = (seq_t)n * k;   /* compute n*k first */
        term -= k;                   /* subtract k   */
        term += 1;                   /* add 1        */
        res *= term;                 /* accumulate product */
    }
    return res;
}
