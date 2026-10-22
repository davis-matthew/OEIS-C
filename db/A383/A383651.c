/*
 * A383651  Expansion of 1/((1-x) * (1+4*x) * (1-6*x)).
 * https://oeis.org/A383651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 22*a(n-2) - 24*a(n-3) * with initial values:
 *            a(0)=1, a(1)=3, a(2)=31
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)31;

    seq_t a0 = (seq_t)1;   // a(n-3)
    seq_t a1 = (seq_t)3;   // a(n-2)
    seq_t a2 = (seq_t)31;  // a(n-1)
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        /* next = 3*a2 + 22*a1 - 24*a0 */
        next = (seq_t)3 * a2 + (seq_t)22 * a1 - (seq_t)24 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
