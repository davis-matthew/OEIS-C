/*
 * A352247  Number of minimum edge cuts in the n-Fibonacci cube graph.
 * https://oeis.org/A352247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   piecewise a(n) = 1 , if n % 3 == 0 a(n) = floor(n/3) + 2 , if n
 *            % 3 == 2 a(1) = 1 a(4) = 7 a(n) (n % 3 == 1, n > 4) : let m = (n
 *            + 2) / 3; a(n) = m*(m+1)/2 + m
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n % 3 == 0)
        return (seq_t)1;
    if (n % 3 == 2)
        return ((seq_t)(n / 3)) + (seq_t)2;
    /* n % 3 == 1 */
    if (n == 1)
        return (seq_t)1;
    if (n == 4)
        return (seq_t)7;
    idx_t m   = (n + 2) / 3;   /* m >= 3 for all remaining cases */
    seq_t mm      = (seq_t)m;
    seq_t tri     = mm * (mm + 1) / 2;   /* triangular number T(m) */
    return tri + mm;
}
