/*
 * A355175  Determinant of the n X n matrix [(i-j)^2 + d(i,j)]_{1<=i,j<=n},
 *          where d(i,j) is 1 or 0 according as i = j or not.
 * https://oeis.org/A355175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n^2-4)*(n^2+2*n+3)*(n^5-2*n^4-n^3-28*n^2+60*n-90)/1080
 * Range:     i8 none, i16 n <= 3, i32 n <= 10, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term1 = nn * nn - 4;                     // n^2 - 4
    seq_t term2 = nn * nn + 2 * nn + 3;            // n^2 + 2n + 3

    seq_t n2 = nn * nn;                            // n^2
    seq_t n3 = n2 * nn;                            // n^3
    seq_t n4 = n3 * nn;                            // n^4
    seq_t n5 = n4 * nn;                            // n^5

    seq_t term3 = n5 - 2 * n4 - n3 - 28 * n2 + 60 * nn - 90;

    seq_t num = term1 * term2;
    num = num * term3;

    return num / 1080;
}
