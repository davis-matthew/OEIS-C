/*
 * A356563  Sums of powers of roots of x^3 - 2*x^2 - x - 2.
 * https://oeis.org/A356563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 2*a(n-3); initial terms: a(0)=3,
 *            a(1)=2, a(2)=6 Computes the term at index n for any n ≥ 0
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 3;
    if (n == 1) return 2;
    if (n == 2) return 6;

    seq_t a0 = 3;
    seq_t a1 = 2;
    seq_t a2 = 6;
    seq_t a3;

    for (idx_t i = 3; i <= n; ++i) {
        a3 = (seq_t)2 * a2 + a1 + (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
