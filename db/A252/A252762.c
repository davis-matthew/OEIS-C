/*
 * A252762  Numbers n such that the sum of the pentagonal numbers P(n),
 *          P(n+1), P(n+2) and P(n+3) is equal to the hexagonal number H(m)
 *          for some m.
 * https://oeis.org/A252762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 195*a(n-1) - 195*a(n-2) + a(n-3) for n>3, with initial
 *            values a(1)=3, a(2)=853, a(3)=165735
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)853;
    if (n == 3) return (seq_t)165735;

    seq_t a1 = (seq_t)3;
    seq_t a2 = (seq_t)853;
    seq_t a3 = (seq_t)165735;
    const seq_t coeff = (seq_t)195;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = coeff * a3 - coeff * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
