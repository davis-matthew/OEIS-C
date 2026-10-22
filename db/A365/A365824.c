/*
 * A365824  a(n) = a(n-1) + 5*a(n-2), for n >= 0, with a(0) = 1 and a(1) = 0.
 * https://oeis.org/A365824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 5*a(n-2), a(0) = 1, a(1) = 0
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* Iteratively compute the recurrence up to n.
       a0 holds a(i-2), a1 holds a(i-1) during the loop. */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)0;   /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t a2 = a1 + (seq_t)5 * a0;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
