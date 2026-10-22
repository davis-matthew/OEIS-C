/*
 * A363571  Expansion of (1 / sqrt(1 - 10*x + 9*x^2) - 1 / (1 - x)) / 4.
 * https://oeis.org/A363571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k)^2 * 4^(k-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the terms of OEIS A363571. */
seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == 0)
        return (seq_t)0;

    seq_t result = 0;
    seq_t binom  = 1;          /* C(n,0) */
    seq_t pow4   = 1;          /* 4^(k-1), starts with k=1 => 4^0 */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update binomial coefficient: C(n,k) = C(n,k-1)*(n-k+1)/k */
        binom = binom * (seq_t)(n - k + 1) / (seq_t)k;

        /* Add C(n,k)^2 * 4^(k-1) to result */
        result += binom * binom * pow4;

        /* Prepare 4^(k) for next iteration */
        pow4 *= (seq_t)4;
    }

    return result;
}
