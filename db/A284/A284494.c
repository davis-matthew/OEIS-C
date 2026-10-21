/*
 * A284494  a(n) = A284016(n)^2.
 * https://oeis.org/A284494
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A284016(n)^2 = (2 * C_n)^2 for n ≥ 0, a(-1) = 1 where C_n
 *            is the nth Catalan number
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(-1) is the first term of the sequence. */
    if (n == (idx_t)-1) {
        return 1;
    }

    /* Compute the n-th Catalan number C_n via the recurrence:
       C_0 = 1
       C_{m+1} = C_m * (2*(2*m + 1)) / (m + 2)    (exact division)    */
    seq_t catalan = 1;                     /* C_0 */
    for (idx_t m = 0; m < n; ++m) {
        seq_t mult = (seq_t)2 * (seq_t)(2 * m + 1);
        catalan = catalan * mult / ((seq_t)m + 2);
    }

    /* A284016(n) = 2*C_n for n ≥ 0.  */
    seq_t root = catalan * 2;

    /* Return the square of the root.  */
    return root * root;
}
