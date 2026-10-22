/*
 * A327767  Period 2: repeat [1, -2].
 * https://oeis.org/A327767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = -2 if n even, 1 if n odd (period 2)
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(2*k)  = -2
     * a(2*k+1)=  1   (with offset 1) */
    return (n & 1) ? (seq_t)1 : (seq_t)-2;
}
