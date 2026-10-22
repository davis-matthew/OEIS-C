/*
 * A384172  a(n) = 4^n * n! * binomial(7*n/4,n) * Sum_{k=1..n} 1/(3*n+4*k).
 * https://oeis.org/A384172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=1}^{n} (3*n+4*k) * Σ_{k=1}^{n} 1/(3*n+4*k)
 *            equivalent to a(n) = Σ_{k=1}^{n} Π_{i=1}^{n} (3*n+4*i) /
 *            (3*n+4*k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the product P = ∏_{k=1}^{n} (3*n+4*k) */
    seq_t P = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t d = (seq_t)(3*n + 4*k);
        P = P * d;          /* use operator*; seq_t supports this */
    }

    /* Sum over all quotients P / (3*n+4*k) */
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t d = (seq_t)(3*n + 4*k);
        sum += P / d;       /* exact division, seq_t has operator/ */
    }

    return sum;
}
