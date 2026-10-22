/*
 * A344049  a(n) = KummerU(-2*n, 1, -n).
 * https://oeis.org/A344049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) = (2*n)! * LaguerreL(2*n, -n, -n)
 * Equivalent sum representation:
 *   a(n) = (2*n)! * Σ_{k=0}^{2*n} binomial(2*n, k) * n^k / k!
 * The implementation computes the sum iteratively while avoiding
 * explicit factorials of all intermediate values.
 */
seq_t a(idx_t n) {
    /* compute term_0 = (2*n)! */
    seq_t term = 1;
    idx_t two_n = 2 * n;
    for (idx_t i = 1; i <= two_n; ++i) {
        term *= (idx_t)i;          /* term = i! iteratively, stops at (2*n)! */
    }

    seq_t sum = term;              /* sum = term_0 */

    for (idx_t k = 0; k < two_n; ++k) {
        /* term_{k+1} = term_k * (2*n - k) * n / ((k+1)^2) */
        term *= (seq_t)(two_n - k);          /* multiply by (2n - k)   */
        term *= (seq_t)n;                    /* multiply by n          */
        idx_t div = (k + 1) * (k + 1);       /* (k+1)^2               */
        term /= (seq_t)div;                  /* exact division        */
        sum += term;                         /* accumulate */
    }
    return sum;
}
