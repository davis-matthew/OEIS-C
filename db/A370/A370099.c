/*
 * A370099  a(n) = Sum_{k=0..n} binomial(2*n,k) * binomial(3*n-k-1,n-k).
 * https://oeis.org/A370099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n,k) * binomial(3*n-k-1,n-k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition. */
    if (n == 0)
        return (seq_t)1;

    /* Pre‑compute 2*n once. */
    idx_t two_n = 2 * n;

    seq_t res        = 0;   /* The sum of terms. */
    seq_t binom2     = 1;   /* C(2*n,0). */
    seq_t binom_n1   = 1;   /* C(n-1,0). */
    seq_t pow2       = 1;   /* 2^0. */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update C(2*n, k) from C(2*n, k-1). */
        binom2 = binom2 * ((seq_t)(two_n - (k - 1))) / ((seq_t)k);

        /* Update 2^k from 2^(k-1). */
        pow2 = pow2 * 2;

        /* Update C(n-1, k-1) from C(n-1, k-2) for k > 1. */
        if (k > 1) {
            idx_t num = (n - 1) - (k - 2);  /* factor for the numerator. */
            binom_n1 = binom_n1 * ((seq_t)num) / ((seq_t)(k - 1));
        }

        /* term = C(2*n, k) * C(n-1, k-1) * 2^k. */
        seq_t term = binom2 * binom_n1 * pow2;
        res += term;
    }

    return res;
}
