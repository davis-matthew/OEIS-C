/*
 * A322888  Chebyshev T-polynomials T_n(16).
 * https://oeis.org/A322888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 16, a(n) = 32*a(n-1) - a(n-2) for n > 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)16;

    /* Iterative computation using the recurrence */
    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)16;  /* a(n-1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = prev1 * (seq_t)32 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
