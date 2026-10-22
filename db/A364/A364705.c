/*
 * A364705  Expansion of 1/(1 - 4*x - x^2 + x^3).
 * https://oeis.org/A364705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - a(n-3), a(0)=1, a(1)=4, a(2)=17
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)17;

    seq_t a0 = (seq_t)1; /* a(n-3) */
    seq_t a1 = (seq_t)4; /* a(n-2) */
    seq_t a2 = (seq_t)17;/* a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = 4 * a2 + a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
