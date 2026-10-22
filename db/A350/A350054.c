/*
 * A350054  a(n) = (4^(3*n+2) - 7)/9.
 * https://oeis.org/A350054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(6*n+4) - 7)/9, recurrence a(n) = 65*a(n-1) -
 *            64*a(n-2), with a(1) = 113, a(2) = 7281
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the behavior is irrelevant; we simply return 0. */
    if (n == 0) return 0;

    /* Handle base cases directly. */
    if (n == 1) return (seq_t)113;
    if (n == 2) return (seq_t)7281;

    /* Iteratively compute the sequence using the linear recurrence. */
    seq_t a_prev2 = (seq_t)113;   /* a(1) */
    seq_t a_prev1 = (seq_t)7281;  /* a(2) */
    seq_t a_cur   = 0;

    for (idx_t k = 3; k <= n; ++k)
    {
        a_cur = (seq_t)65 * a_prev1 - (seq_t)64 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }
    return a_cur;
}
