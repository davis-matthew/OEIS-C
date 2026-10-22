/*
 * A349640  a(n) = Sum_{k=0..n} binomial(n,k) * A000108(k) * k!.
 * https://oeis.org/A349640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * Catalan(k) * k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum   = 0;     /* result */
    seq_t binom = 1;     /* binomial(n,k)   */
    seq_t fact  = 1;     /* k!              */
    seq_t cat   = 1;     /* Catalan(k)      */
    idx_t k;

    for (k = 0; k <= n; ++k) {
        /* term = binom * fact * cat */
        seq_t term = binom;
        term *= fact;
        term *= cat;
        sum += term;

        if (k == n)
            break;

        /* update values for next k */
        seq_t next_binom = binom * ((seq_t)(n - k)) / ((seq_t)(k + 1));
        seq_t next_cat   = cat * ((seq_t)(4 * (k + 1) - 2)) / ((seq_t)(k + 2));
        seq_t next_fact  = fact * ((seq_t)(k + 1));

        binom = next_binom;
        cat   = next_cat;
        fact  = next_fact;
    }

    return sum;
}
