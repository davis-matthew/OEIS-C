/*
 * A331413  a(n) = 2*a(n-1) + a(n-2) + a(n-3) + a(n-4), a(0)=4, a(1)=2,
 *          a(2)=6, a(3)=17.
 * https://oeis.org/A331413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + a(n-3) + a(n-4); a(0) = 4, a(1) = 2,
 *            a(2) = 6, a(3) = 17
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)17;

    seq_t a0 = 4;   /* a(i-4) */
    seq_t a1 = 2;   /* a(i-3) */
    seq_t a2 = 6;   /* a(i-2) */
    seq_t a3 = 17;  /* a(i-1) */
    seq_t next = 0;

    for (idx_t i = 4; i <= n; ++i) {
        next = 2 * a3 + a2 + a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return next;
}
