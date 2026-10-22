/*
 * A367456  Expansion of (1 - x)/(1 - x - 7*x^2).
 * https://oeis.org/A367456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 7·a(n-2), a(0) = 1, a(1) = 0
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of A367456.
 * Works for any seq_t type, including arbitrary-precision integer classes,
 * and for any idx_t unsigned integer type. */
seq_t a(idx_t n)
{
    /* Handle base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* Run recurrence iteratively without any dynamic allocation. */
    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)0;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 + (seq_t)7 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
