/*
 * A258684  a(n) = A041105(4n+1).
 * https://oeis.org/A258684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 62*a(n-1) - a(n-2), a(0) = 1, a(1) = 63
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == (idx_t)0)
        return (seq_t)1;
    if (n == (idx_t)1)
        return (seq_t)63;

    /* Initialise the two previous values of the recurrence. */
    seq_t prev2 = (seq_t)1;      /* a(n-2) */
    seq_t prev1 = (seq_t)63;     /* a(n-1) */
    seq_t curr;

    /* Iterate the linear recurrence up to the requested index. */
    for (idx_t i = (idx_t)2; i <= n; ++i) {
        curr = prev1 * (seq_t)62u - prev2;  /* a(i) = 62*a(i-1) – a(i-2) */
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
