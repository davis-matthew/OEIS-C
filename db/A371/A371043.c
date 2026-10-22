/*
 * A371043  E.g.f. satisfies A(x) = 1 + x^2*A(x)*exp(x*A(x)).
 * https://oeis.org/A371043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} k^(n-2*k) *
 *            binomial(n-k+1,k)/( (n-k+1)*(n-2*k)! )
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation computes the sum iteratively, using
   multiplicative formulas for the binomial coefficient and the
   factorial product.  Exponentiation is performed by binary
   exponentiation on seq_t.  The routine works for any seq_t
   (fixed‑size unsigned integer or arbitrary precision). */

seq_t a(idx_t n)
{
    /* Handle the offset n = 0 directly */
    if (n == 0)
        return (seq_t)1;

    idx_t half = n / 2;          /* floor(n/2) */
    seq_t sum = (seq_t)0;        /* accumulator for the sum part */
    seq_t P   = (seq_t)1;        /* factorial product n!/(n-2k)!   */
    seq_t B   = (seq_t)0;        /* current binomial value        */

    if (half >= 1)
        B = (seq_t)n;            /* for k = 1: binomial(n,1) = n */

    for (idx_t k = 1; k <= half; ++k) {
        /* Update the factorial part P *= (n-2k+1)*(n-2k+2)   */
        seq_t f1 = (seq_t)(n - 2*k + 1);
        seq_t f2 = (seq_t)(n - 2*k + 2);
        P = P * f1 * f2;

        /* Compute Q = B * P / (n - k + 1)      */
        seq_t Q = B * P;
        seq_t denom = (seq_t)(n - k + 1);
        Q = Q / denom;             /* division is exact */

        /* Compute k^(n-2k) using binary exponentiation */
        seq_t pow_res = (seq_t)1;
        idx_t exp = n - 2*k;
        if (exp != 0) {
            seq_t base = (seq_t)k;
            seq_t b = base;
            while (exp > 0) {
                if (exp & 1) pow_res = pow_res * b;
                b = b * b;
                exp >>= 1;
            }
        }

        /* term = k^(n-2k) * Q */
        seq_t term = pow_res * Q;
        sum += term;

        /* If we are not at the last k, update B for the next iteration */
        if (k < half) {
            /* B_next = B * (n - 2*k) * (n - 2*k + 1) / ((n - k + 1)*(k + 1)) */
            seq_t n_minus_2k = (seq_t)(n - 2*k);
            seq_t n_minus_2k_plus1 = (seq_t)(n - 2*k + 1);
            seq_t zden1 = (seq_t)(n - k + 1);
            seq_t zden2 = (seq_t)(k + 1);
            B = (B * n_minus_2k * n_minus_2k_plus1) / (zden1 * zden2);
        }
    }

    return sum;
}
