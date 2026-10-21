/*
 * A253504  Number of (n+2) X (2+2) 0..1 arrays with every 2 X 2 and 3X3
 *          subblock diagonal maximum minus antidiagonal minimum nondecreasing
 *          horizontally and vertically.
 * https://oeis.org/A253504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n^3 + 108*n^2 + 332*n + 276) / 3 for n >= 3 a(1) =
 *            260, a(2) = 472
 * Range:     u8 none, u16 n <= 18, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)260;
    if (n == 2) return (seq_t)472;
    seq_t s = (seq_t)n;
    seq_t val = 4 * s * s * s + 108 * s * s + 332 * s + 276;
    return val / 3;
}
