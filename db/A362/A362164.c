/*
 * A362164  Expansion of e.g.f. -exp(x * (1-3*x)^(1/3)).
 * https://oeis.org/A362164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..n} (-1)^{k+1} * C(n,k) * Π_{i=0}^{k-1}
 *            (n-k-3*i)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom = 1; /* C(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        /* compute product Π_{i=0}^{k-1} (n-k-3*i) */
        seq_t prod = 1;
        for (idx_t i = 0; i < k; ++i) {
            prod *= ( (seq_t)n - (seq_t)k - (seq_t)3 * (seq_t)i );
        }

        /* sign = (-1)^{k+1} */
        seq_t sign = (k & 1) ? 1 : -1;

        /* term = sign * binom * prod */
        seq_t term = prod * binom;
        term *= sign;

        sum += term;

        /* update binom for next k: C(n,k+1) = C(n,k) * (n-k)/(k+1) */
        if (k < n) {
            binom = binom * ((seq_t)n - (seq_t)k) / ((seq_t)(k + 1));
        }
    }

    return sum;
}
