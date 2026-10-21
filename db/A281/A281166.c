/*
 * A281166  a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) for n>2, a(0)=a(1)=1,
 *          a(2)=3.
 * https://oeis.org/A281166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) for n>2, a(0)=1, a(1)=1,
 *            a(2)=3
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 32, u64 n <= 63, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)3;

    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)1;   /* a(n-2) */
    seq_t a2 = (seq_t)3;   /* a(n-1) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = 3 * a2 - 3 * a1 + 2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
