/*
 * A244895  Period 5: repeat [0, 1, 1, -1, -1].
 * https://oeis.org/A244895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = periodic pattern [0, 1, 1, -1, -1], i.e. a(5k)=0,
 *            a(5k+1)=a(5k+2)=1, a(5k+3)=a(5k+4)=-1
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pat[5] = { (seq_t)0, (seq_t)1, (seq_t)1, (seq_t)-1, (seq_t)-1 };
    return pat[n % 5];
}
