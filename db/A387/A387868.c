/*
 * A387868  a(n) = Sum_{k=0..n} binomial(2*n+1,3*k).
 * https://oeis.org/A387868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) + 4*a(n-3) for n > 2, with initial
 *            values a(0) = 1, a(1) = 2, a(2) = 11
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)11;

    /* a0, a1, a2 hold the last three computed terms. */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)2;
    seq_t a2 = (seq_t)11;
    seq_t ax;

    for (idx_t i = 3; i <= n; ++i) {
        ax = 3 * a2 + 3 * a1 + 4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = ax;
    }
    return a2;
}
