/*
 * A368840  a(n) = n! * (n+1)! * (n+2)! * Sum_{k=0..n} 1/(6^(n-k) * k! *
 *          (k+1)! * (k+2)!).
 * https://oeis.org/A368840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+2,3)*a(n-1) + 1, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a_n = (seq_t)1;          /* a(0) */
    if (n == 0) return a_n;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* binomial(i+2,3) = (i+2)*(i+1)*i / 6 */
        seq_t binom = ((seq_t)(i + 2) * (seq_t)(i + 1) * (seq_t)i) / (seq_t)6;
        a_n = binom * a_n + (seq_t)1;
    }

    return a_n;
}
