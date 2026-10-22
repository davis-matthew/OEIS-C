/*
 * A385668  a(n) = Sum_{k=0..n} 2^(n-k) * binomial(4*n+1,k) *
 *          binomial(4*n-k,n-k).
 * https://oeis.org/A385668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * binomial(3*n + k, k) (OEIS A385668)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* local variables */
    seq_t pow3 = 1;      /* 3^k */
    seq_t binom = 1;     /* binomial(3*n + k, k) */
    seq_t sum = 0;       /* accumulator for a(n) */

    /* iterate over k = 0 .. n */
    for (idx_t k = 0; k <= n; ++k) {
        /* add current term: 3^k * binomial(3*n + k, k) */
        seq_t term = pow3 * binom;
        sum += term;

        /* stop after last term */
        if (k == n) break;

        /* update 3^k to 3^(k+1) */
        pow3 *= 3;

        /* update binomial: binom(3*n + (k+1), k+1) = binom * (3*n + k + 1)/(k + 1) */
        seq_t num   = (seq_t)(3 * n + k + 1);
        seq_t denom = (seq_t)(k + 1);
        binom = (binom * num) / denom;   /* exact division */
    }

    return sum;
}
