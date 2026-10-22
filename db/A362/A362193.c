/*
 * A362193  Number of Grassmannian permutations of size n that avoid a
 *          pattern, sigma, where sigma is a pattern of size 6 with exactly
 *          one descent.
 * https://oeis.org/A362193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Σ_{i=2}^{5} binomial(n,i) (binomial(n,i)=0 for i>n)
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 206, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases: a(0)=1, a(1)=1 */
    if (n < 2)
        return (seq_t)1;

    seq_t total = (seq_t)1;          /* the initial "+1" term */

    /* Sum binomial(n,i) for i = 2..5, but stop if i > n */
    for (idx_t k = 2; k <= 5; ++k)
    {
        if (k > n)
            break;                     /* further terms are zero */

        seq_t bin = (seq_t)1;
        for (idx_t i = 1; i <= k; ++i)
        {
            /* compute bin = bin * (n - i + 1) / i, exact division for each step */
            bin = bin * (seq_t)(n - i + 1) / (seq_t)i;
        }
        total += bin;
    }

    return total;
}
