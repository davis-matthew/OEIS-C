/*
 * A259907  Fifth differences of 7th powers (A001015).
 * https://oeis.org/A259907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=123, a(2)=1557, a(3)=6719, * a(n) = 840*(3*n^2 -
 *            9*n + 8) for n>3 * (equivalently a(n) = 3*a(n-1) - 3*a(n-2) +
 *            a(n-3) for n>6)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[4] = { (seq_t)1, (seq_t)123, (seq_t)1557, (seq_t)6719 };
    if (n < 4) return init[n];
    seq_t nt = (seq_t)n;
    seq_t val = (seq_t)840 * ((seq_t)3 * nt * nt - (seq_t)9 * nt + (seq_t)8);
    return val;
}
