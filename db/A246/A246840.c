/*
 * A246840  Expansion of Sum_{n>=0} x^n * Sum_{k=0..n} C(n,k)^2 * x^(2*k).
 * https://oeis.org/A246840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{⌊n/3⌋} binomial(n-2k, k)^2 (binomial is the
 *            ordinary binomial coefficient)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 32, u64 n <= 61, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sum is over k such that n-2k ≥ k,
       i.e. k ≤ n/3.  For k > n/3 all terms are zero. */
    idx_t max_k = n / 3;
    seq_t sum = 1;   /* k = 0 term: C(n,0)^2 = 1     */
    seq_t f   = 1;   /* f = C(n-2k, k), start with k=0 */

    for (idx_t k = 0; k < max_k; ++k) {
        /* Compute C(n-2(k+1), k+1) from C(n-2k, k) */
        idx_t s = n - 3 * k;                                 /* s = n‑3k  */
        seq_t numer = (seq_t)s * (s - 1) * (s - 2);          /* (n-3k)(n-3k-1)(n-3k-2) */
        seq_t denom = (seq_t)(k + 1) * (n - 2 * k) * (n - 2 * k - 1);
        f = f * numer / denom;                              /* f_{k+1}   */
        sum += f * f;                                       /* add f^2    */
    }
    return sum;
}
