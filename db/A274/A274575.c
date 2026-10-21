/*
 * A274575  For m=1,2,3,... write all the 2^m binary vectors of length m in
 *          increasing order, and replace each vector with (number of 1's) -
 *          (number of 0's). Start with an initial 0 for the empty vector.
 * https://oeis.org/A274575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=0; a(2*n+1)=a(n)-1; a(2*n+2)=a(n)+1
 * Range:     i8 n <= 8190, i16 n <= 8190, i32 n <= 8190, i64 n <= 8190,
 *            i128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* base case */
    if ((n & 1) != 0)                   /* odd: n = 2*m+1 */
        return a((n - 1) / 2) - 1;
    /* even (n > 0): n = 2*m+2 */
    return a((n - 2) / 2) + 1;
}
