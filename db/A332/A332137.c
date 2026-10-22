/*
 * A332137  a(n) = (10^(2n+1)-1)/3 + 4*10^n.
 * https://oeis.org/A332137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10^(2n+1)-1)/3 + 4*10^n
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)7;
    if (n == 1) return (seq_t)373;
    if (n == 2) return (seq_t)33733;

    seq_t a0 = (seq_t)7;
    seq_t a1 = (seq_t)373;
    seq_t a2 = (seq_t)33733;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)111 * a2 - (seq_t)1110 * a1 + (seq_t)1000 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
