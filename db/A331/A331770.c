/*
 * A331770  El Gradechi's hybrid coefficients alpha^{6,6}_{2n}, negated.
 * https://oeis.org/A331770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1008 * binomial(5 + 2*n, 5)
 * Range:     u8 none, u16 n <= 1, u32 n <= 26, u64 n <= 2328, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t bin = 1;                       /* start with C(2n+5,0) */
    seq_t n_seq = (seq_t)n;              /* cast index to seq_t once */
    seq_t base = n_seq * 2 + 5;          /* 2n + 5  */

    for (idx_t k = 1; k <= 5; ++k) {
        /* multiply by (2n+6-k) and divide by k to keep the
           intermediate value integral at every step   */
        bin *= (base + 1 - k);
        bin /= k;
    }
    return ((seq_t)1008) * bin;          /* final scaling by 1008    */
}
