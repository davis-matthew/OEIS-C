/*
 * A362194  Number of Grassmannian permutations of size n that avoid a
 *          pattern, sigma, where sigma is a pattern of size 7 with exactly
 *          one descent.
 * https://oeis.org/A362194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{i=2..6} binomial(n,i)
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 117, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)1;

    idx_t m = (n < 6 ? n : 6);      /* compute up to min(n,6) */
    seq_t binom = (seq_t)1;         /* C(n,0) = 1   */
    seq_t sum   = (seq_t)0;

    for (idx_t i = 1; i <= m; ++i) {
        binom = binom * (seq_t)(n - i + 1) / (seq_t)i;  /* C(n,i) from C(n,i-1) */
        if (i >= 2)
            sum += binom;
    }

    return sum + (seq_t)1;
}
