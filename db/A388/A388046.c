/*
 * A388046  a(n) = Sum_{k=0..n} 2^k * binomial(n,k) * binomial(4*n+1,k).
 * https://oeis.org/A388046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^k * binomial(n,k) * binomial(4*n+1,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A388046 by a simple O(n) recurrence, suitable for any seq_t size. */
seq_t a(idx_t n)
{
    /* n is an idx_t (uint64_t).  Use seq_t arithmetic for all values. */
    seq_t result = 0;
    seq_t pow2   = 1;                     /* 2^k */
    seq_t bin1   = 1;                     /* binomial(n,k)   */
    seq_t bin2   = 1;                     /* binomial(4*n+1,k) */
    seq_t N = (seq_t)(4 * (seq_t)n + (seq_t)1); /* 4*n+1 as seq_t */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = 2^k * binomial(n,k) * binomial(4*n+1,k) */
        seq_t term = bin1;
        term = term * bin2;
        term = term * pow2;
        result = result + term;

        if (k == n) break;  /* avoid needless updates after last term */

        /* Update binom(n,k) -> binom(n,k+1) :  binom(n,k+1) = binom(n,k)*(n-k)/(k+1) */
        bin1 = bin1 * ((seq_t)n - (seq_t)k);
        bin1 = bin1 / ((seq_t)k + (seq_t)1);

        /* Update binom(4*n+1,k) -> binom(4*n+1,k+1) */
        bin2 = bin2 * (N - (seq_t)k);
        bin2 = bin2 / ((seq_t)k + (seq_t)1);

        /* Update 2^k -> 2^(k+1) */
        pow2 = pow2 * (seq_t)2;
    }

    return result;
}
