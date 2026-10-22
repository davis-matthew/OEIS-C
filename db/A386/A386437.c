/*
 * A386437  Alternating sum of the numbers in the trajectory of n for the 3x+1
 *          problem.
 * https://oeis.org/A386437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = Sum_{k=1..A006577(n)} A070165(n,k) * (-1)^(k+1) This
 *            function computes the alternating sum of the Collatz (3x+1)
 *            trajectory of n. The trajectory includes n itself and ends with
 *            1. The signs alternate starting with + for the first term
 * Range:     i8 n <= 14, i16 n <= 446, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t t = (seq_t)n;
    int sign = 1;
    while (1) {
        sum += t * sign;
        if (t == 1) break;
        if ((t & 1) == 0)
            t >>= 1;           /* even case: x = x / 2 */
        else
            t = t * 3 + 1;     /* odd case:  x = 3*x + 1 */
        sign = -sign;          /* flip sign for next term */
    }
    return sum;
}
