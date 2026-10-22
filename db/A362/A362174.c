/*
 * A362174  Number of n X n matrices with nonnegative integer entries such
 *          that the sum of the elements of each row is equal to the index of
 *          that row.
 * https://oeis.org/A362174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} binomial(n + k - 1, k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t result = (seq_t)1;
    /* b = binom(n + 1 - 1, 1) = n  */
    seq_t b = (seq_t)n;

    for (idx_t i = 1; i <= n; ++i) {
        result *= b;

        if (i == n) {
            break;              /* avoid one more update after last multiplication */
        }

        /* Update b to binom(n + i, i + 1)
           Uses the recurrence: B_{i+1} = B_i * (n + i) / (i + 1)     */
        seq_t numerator = b * (seq_t)(n + i);
        seq_t denom     = (seq_t)(i + 1);
        b = numerator / denom;   /* exact division */
    }

    return result;
}
