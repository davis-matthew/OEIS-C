/*
 * A386960  a(n) = Sum_{k=0..n} 8^k * binomial(2*n,n-k).
 * https://oeis.org/A386960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 8^k * binomial(2*n, n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute central binomial coefficient C(2n, n) */
    seq_t bin = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        bin = bin * ((seq_t)n + (seq_t)i);
        bin = bin / ((seq_t)i);
    }

    seq_t sum  = (seq_t)0;
    seq_t pow8 = (seq_t)1; /* 8^k */
    seq_t B    = bin;      /* current binomial C(2n, n-k) */

    for (idx_t k = 0; k <= n; ++k) {
        sum += pow8 * B;

        if (k != n) {
            /* advance to next term */
            pow8 *= (seq_t)8;
            B = B * ((seq_t)(n - k));
            B = B / ((seq_t)(n + k + 1));
        }
    }
    return sum;
}
