/*
 * A335719  a(n) = 2*a(n-1) + 3*a(n-2) + 5*a(n-3), a(0) = 3, a(1) = 2, a(2) =
 *          10.
 * https://oeis.org/A335719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) + 5*a(n-3), a(0) = 3, a(1) = 2, a(2)
 *            = 10
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)10;

    seq_t a0 = (seq_t)3;  /* a(0) */
    seq_t a1 = (seq_t)2;  /* a(1) */
    seq_t a2 = (seq_t)10; /* a(2) */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t next = 2 * a2 + 3 * a1 + 5 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
