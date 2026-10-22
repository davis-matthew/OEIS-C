/*
 * A309970  Period 12: repeat [1, 1, 0, 0, -1, -1, -1, -1, 0, 0, 1, 1].
 * https://oeis.org/A309970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-2) - a(n-4) for n>3 with a(0)=1,a(1)=1,a(2)=0,a(3)=0;
 *            period 12 [1,1,0,0,-1,-1,-1,-1,0,0,1,1]
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[12] = {1, 1, 0, 0, -1, -1, -1, -1, 0, 0, 1, 1};
    return pattern[n % 12];
}
