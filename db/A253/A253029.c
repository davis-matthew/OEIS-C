/*
 * A253029  Number of (n+2) X (1+2) 0..2 arrays with every consecutive three
 *          elements in every row and column having exactly two distinct
 *          values, and new values 0 upwards introduced in row major order.
 * https://oeis.org/A253029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 3*a(n-2), a(1) = 297, a(2) = 1647
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)297;
    if (n == 2) return (seq_t)1647;

    seq_t a_prev2 = (seq_t)297;   /* a(1) */
    seq_t a_prev1 = (seq_t)1647;  /* a(2) */
    seq_t a_curr;

    for (idx_t i = 3; i <= n; ++i) {
        a_curr = (seq_t)5 * a_prev1 + (seq_t)3 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
