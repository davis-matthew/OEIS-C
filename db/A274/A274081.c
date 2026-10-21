/*
 * A274081  Number of unrooted labeled trees on 3n+2 nodes with node degree
 *          either one or four.
 * https://oeis.org/A274081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(3n+2, n) * (3n)! / 6^n, where binomial(·,·) is
 *            the ordinary binomial coefficient. The implementation evaluates
 *            the binomial coefficient multiplicatively, cancels powers of 2
 *            and 3 with the denominator 6^n while building the factorial to
 *            keep the intermediate values as small as pos...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case directly */
    if (n == 0) return (seq_t)1;

    /* Step 1: compute binomial(3n+2, n) */
    seq_t binom = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t num = (seq_t)(2 * n + 2 + i);          /* (2n+2 + i) */
        binom = binom * num / (seq_t)i;              /* exact division */
    }

    /* Step 2: cancel powers of 2 and 3 from the binomial part */
    idx_t twoNeeded   = n;  /* one factor of 2 from each 6 in 6^n */
    idx_t threeNeeded = n;  /* one factor of 3 from each 6 in 6^n */

    while (twoNeeded > 0 && (binom % 2 == 0))
    {
        binom /= 2;
        --twoNeeded;
    }
    while (threeNeeded > 0 && (binom % 3 == 0))
    {
        binom /= 3;
        --threeNeeded;
    }

    /* Step 3: multiply by (3n)! cancelling remaining 2s and 3s */
    seq_t result = binom;   /* start with the partially reduced binomial */

    for (idx_t j = 1; j <= 3 * n; ++j)
    {
        seq_t v = j;

        while (twoNeeded > 0 && (v % 2 == 0))
        {
            v /= 2;
            --twoNeeded;
        }
        while (threeNeeded > 0 && (v % 3 == 0))
        {
            v /= 3;
            --threeNeeded;
        }

        result *= v;
    }

    /* In theory twoNeeded and threeNeeded should be zero.
       If not, multiply the remaining small factors. */
    for (idx_t i = 0; i < twoNeeded; ++i)   result *= 2;
    for (idx_t i = 0; i < threeNeeded; ++i) result *= 3;

    return result;
}
