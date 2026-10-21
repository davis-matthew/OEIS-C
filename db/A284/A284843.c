/*
 * A284843  Number of permutations on [n+2] with no circular 2-successions.
 * https://oeis.org/A284843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+2) * Sum_{j=0..n} (-1)^j * (n - j + 1) * n! / j!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Compute n! (no memoisation needed) */
    seq_t fact = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    seq_t current = fact;      /* will be updated to fact/j! during the loop */
    seq_t sum     = (seq_t)0;
    int   sign    = 1;          /* +1 for even j, -1 for odd j */

    for (idx_t j = 0; j <= n; ++j) {
        seq_t term = (seq_t)(n - j + 1) * current;
        if (sign < 0)
            sum -= term;
        else
            sum += term;

        sign = -sign;           /* toggle sign for next j */

        /* Prepare current = fact/(j+1)! for next iteration */
        if (j < n)
            current /= (seq_t)(j + 1);
    }

    seq_t result = (seq_t)(n + 2) * sum;
    return result;
}
