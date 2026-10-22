/*
 * A342710  Solutions x to the Pell-Fermat equation x^2 - 5*y^2 = 4.
 * https://oeis.org/A342710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - a(n-2), with a(0) = 3, a(1) = 18
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)18;

    seq_t a_prev2 = (seq_t)3;   /* a(0) */
    seq_t a_prev1 = (seq_t)18;  /* a(1) */
    seq_t a_curr;

    for (idx_t i = 2; i <= n; ++i) {
        a_curr = (seq_t)7 * a_prev1 - a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}
