/*
 * A329511  Expansion of (1 + x)*(1 + x^2)^2*(1 + x - x^3) / (1 - x).
 * https://oeis.org/A329511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = cumulative sum of coefficients of the numerator * of
 *            (1+x)*(1+x^2)^2*(1+x-x^3), i.e. a(0)=1, a(1)=3, a(2)=6, *
 *            a(3)=9, a(4)=11, a(5)=11, a(6)=10, a(7)=9, a(8)=8, and a(n)=8
 *            for n≥8
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t values[9] = {1, 3, 6, 9, 11, 11, 10, 9, 8};
    if (n < 9)
        return values[n];
    else
        return 8;
}
