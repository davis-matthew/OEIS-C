/*
 * A281199  Number of n X 2 0..1 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=2, a(3)=10, a(4)=38 a(n) = 6*a(n-1) - 11*a(n-2) +
 *            6*a(n-3) - a(n-4) for n ≥ 5 (empirical recurrence)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)10;
    if (n == 4) return (seq_t)38;

    seq_t prev4 = (seq_t)0;   /* a(n-4)   */
    seq_t prev3 = (seq_t)2;   /* a(n-3)   */
    seq_t prev2 = (seq_t)10;  /* a(n-2)   */
    seq_t prev1 = (seq_t)38;  /* a(n-1)   */
    seq_t cur;

    for (idx_t i = 5; i <= n; ++i) {
        cur = 6 * prev1;     /* 6*a(n-1) */
        cur -= 11 * prev2;    /* - 11*a(n-2) */
        cur += 6 * prev3;     /* + 6*a(n-3) */
        cur -= prev4;        /* - a(n-4)   */

        /* Shift the window for the next iteration */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
