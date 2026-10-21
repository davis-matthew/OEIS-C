/*
 * A275229  Number of 3 X n 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-2,1) or (-1,0) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 8*a(n-2), a(1) = 2, a(2) = 11. (OEIS A275229)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For safety, return 0 for indices < 1 (not used by the OEIS tests). */
    if (n < 1) return (seq_t)0;

    /* Handle the first two base cases. */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)11;

    /* Compute the recurrence iteratively. */
    seq_t prev2 = (seq_t)2;   /* a(1) */
    seq_t prev1 = (seq_t)11;  /* a(2) */
    seq_t cur   = (seq_t)0;

    for (idx_t i = 3; i <= n; ++i) {
        cur   = (seq_t)(7 * prev1 - 8 * prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
