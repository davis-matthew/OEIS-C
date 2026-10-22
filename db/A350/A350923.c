/*
 * A350923  a(0) = 2, a(1) = 2, and a(n) = 10*a(n-1) - a(n-2) - 4 for n >= 2.
 * https://oeis.org/A350923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - a(n-2) - 4, a(0) = 2, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial initial values */
    if (n == 0) return 2;
    if (n == 1) return 2;

    /* Iterate once for each subsequent index */
    seq_t a_prev2 = 2;  /* a(n-2) */
    seq_t a_prev1 = 2;  /* a(n-1) */
    seq_t a_curr;       /* a(n) to be computed */

    for (idx_t i = 2; i <= n; ++i) {
        a_curr = 10 * a_prev1 - a_prev2 - 4;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
