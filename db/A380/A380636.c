/*
 * A380636  Expansion of e.g.f. exp(x*C(2*x)^2) where C(x) = 1 + x*C(x)^2 is
 *          the g.f. of the Catalan numbers, A000108.
 * https://oeis.org/A380636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 for n > 0: a(n) = (n-1)! * Σ_{k=0}^{n-1} 2^k * C(2n,k)
 *            / (n-k-1)!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A380636. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t sum  = (seq_t)0;     /* accumulated sum of terms */
    seq_t P    = (seq_t)1;     /* falling factorial (n-1)!/(n-k-1)! */
    seq_t pow2 = (seq_t)1;     /* 2^k          */
    seq_t binom = (seq_t)1;    /* C(2n,k)      */

    for (idx_t k = 0; k < n; ++k) {
        seq_t term = P * pow2 * binom;
        sum += term;

        if (k + 1 < n) {
            /* update product for next k: P(k+1) = P(k) * (n-1-k) */
            P = P * (seq_t)(n - 1 - k);

            /* multiply by 2 for 2^(k+1) */
            pow2 = pow2 * (seq_t)2;

            /* compute next binomial coefficient:
               binom(k+1) = binom(k) * (2n - k) / (k+1)
               division is exact            */
            seq_t num = binom * (seq_t)(2 * n - k);
            binom = num / (seq_t)(k + 1);
        }
    }

    return sum;
}
