/*
 * A269685  Number of length-n 0..3 arrays with no repeated value differing
 *          from the previous repeated value by plus or minus one modulo 3+1.
 * https://oeis.org/A269685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2) - 6*a(n-3), with a(1)=4, a(2)=16,
 *            a(3)=64. (OEIS A269685, offset 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Behaviour below the offset is irrelevant. */
    if (n == 0)
        return 0;

    /* Initial terms of the sequence. */
    const seq_t a1 = 4;
    const seq_t a2 = 16;
    const seq_t a3 = 64;

    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    /* Use a simple iterative implementation of the recurrence. */
    seq_t prev3 = a1;   /* a(n-3)  */
    seq_t prev2 = a2;   /* a(n-2)  */
    seq_t prev1 = a3;   /* a(n-1)  */
    seq_t cur   = 0;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 6*a(i-1) - 7*a(i-2) - 6*a(i-3) */
        cur = (seq_t)6 * prev1 - (seq_t)7 * prev2 - (seq_t)6 * prev3;
        /* shift the window for the next iteration */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
