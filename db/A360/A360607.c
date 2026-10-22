/*
 * A360607  a(n) = (n + 1/3) * (3*n + 3)! / ((n + 1)!)^3.
 * https://oeis.org/A360607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + 1/3) * (3*n + 3)! / ((n + 1)!)^3
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n + 1;
    seq_t M = 1;
    for (idx_t k = 1; k <= m; ++k) {
        seq_t kseq = (seq_t)k;
        M *= ((seq_t)3 * kseq - 2);   /* multiply by (3*k - 2) */
        M /= kseq;                   /* divide by k */
        M *= ((seq_t)3 * kseq - 1);   /* multiply by (3*k - 1) */
        M /= kseq;                   /* divide by k */
        M *= ((seq_t)3 * kseq);       /* multiply by (3*k) */
        M /= kseq;                   /* divide by k */
    }
    seq_t result = M * ((seq_t)3 * m - 2); /* multiply by (3*m - 2) */
    result /= 3;                           /* final division by 3 */
    return result;
}
