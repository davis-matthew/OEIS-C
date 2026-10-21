/*
 * A246467  G.f.: 1 / AGM(1-5*x, sqrt((1-x)*(1-25*x))).
 * https://oeis.org/A246467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( sum_{k=0..n} binomial(n,k) * binomial(2k,k) )^2 where
 *            binomial is the usual binomial coefficient. The sequence is
 *            A246467
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute S(n) = sum_{k=0..n} C(n,k) * C(2k,k) */
    seq_t sum   = 0;
    seq_t binom = 1;          /* C(n,0) */
    seq_t central = 1;        /* C(0,0) */

    for (idx_t k = 0; k <= n; ++k) {
        sum += binom * central;

        if (k < n) {
            /* update binomial C(n,k+1) = C(n,k)*(n-k)/(k+1) */
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);

            /* update central C(2(k+1),k+1) from C(2k,k) */
            seq_t num = (seq_t)(2 * k + 2) * (seq_t)(2 * k + 1);
            seq_t den = (seq_t)(k + 1) * (seq_t)(k + 1);
            central = central * num / den;
        }
    }

    return sum * sum;
}
