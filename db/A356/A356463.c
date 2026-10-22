/*
 * A356463  Sum of powers of roots of x^3 - 4*x^2 + x + 1.
 * https://oeis.org/A356463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2) - a(n-3); Initial values: a(0)=3,
 *            a(1)=4, a(2)=14
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms directly */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)14;

    /* Recurrence: a(n) = 4*a(n-1) - a(n-2) - a(n-3) */
    seq_t a0 = (seq_t)3;   /* a(n-3) */
    seq_t a1 = (seq_t)4;   /* a(n-2) */
    seq_t a2 = (seq_t)14;  /* a(n-1) */
    seq_t a_curr = a2;

    /* Iterate from 3 up to n */
    for (idx_t i = 3; i <= n; ++i) {
        a_curr = 4 * a2 - a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a_curr;
    }
    return a_curr;
}
