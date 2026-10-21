/*
 * A248163  Chebyshev's S polynomials (A049310) evaluated at 34/3 and
 *          multiplied by powers of 3 (A000244).
 * https://oeis.org/A248163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*a(n-1) - 9*a(n-2), a(0) = 1, a(-1) = 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is defined explicitly. */
    if (n == 0)
        return (seq_t)1;

    /* a(-1) = 0, a(0) = 1 */
    seq_t prev2 = (seq_t)0; /* a(-1) */
    seq_t prev1 = (seq_t)1; /* a(0) */
    seq_t cur   = (seq_t)0;

    for (idx_t i = 1; i <= n; ++i) {
        /* recurrence: a(i) = 34*a(i-1) - 9*a(i-2) */
        cur = (seq_t)34 * prev1 - (seq_t)9 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
