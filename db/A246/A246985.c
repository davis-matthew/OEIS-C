/*
 * A246985  Expansion of (1-8*x+14*x^2)/((1-2*x)*(1-3*x)*(1-6*x)).
 * https://oeis.org/A246985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 2^(n-1) + 3^(n-1) + 6^(n-1) for n > 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t e = n - 1;          /* exponent to raise the bases to */
    seq_t p2 = (seq_t)1;
    seq_t p3 = (seq_t)1;
    seq_t p6 = (seq_t)1;

    for (idx_t i = 0; i < e; ++i) {
        p2 *= (seq_t)2;
        p3 *= (seq_t)3;
        p6 *= (seq_t)6;
    }

    return p2 + p3 + p6;
}
