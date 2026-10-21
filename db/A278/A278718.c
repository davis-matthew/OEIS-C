/*
 * A278718  Numerators of A189733: periodic sequence repeating [0, 1, 1, 1, 0,
 *          -1].
 * https://oeis.org/A278718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, sign, easy
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5) (n>4), with
 *            a(0)=0, a(1)=1, a(2)=1, a(3)=1, a(4)=0, a(5)=-1. The sequence
 *            repeats with period 6: [0, 1, 1, 1, 0, -1]
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {0, 1, 1, 1, 0, -1};
    return pattern[n % 6];
}
