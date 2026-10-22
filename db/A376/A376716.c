/*
 * A376716  Expansion of (1 - x + x^2)/((1 - x + x^2)^2 - 4*x^2).
 * https://oeis.org/A376716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 2*a(n-3) - a(n-4). Initial values:
 *            a(0)=1, a(1)=1, a(2)=4, a(3)=11
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)11;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)4;
    seq_t a3 = (seq_t)11;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t a4 = 2 * a3 + a2 + 2 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }

    return a3;
}
