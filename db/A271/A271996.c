/*
 * A271996  The crystallogen sequence (a(n) = A018227(n)-4).
 * https://oeis.org/A271996
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*(-9 + (-1)^n) + (25 + 3*(-1)^n)*n + 12*n^2 + 2*n^3)/12
 *            Equivalently: if n even: a(n) = (n^3 + 6*n^2 + 14*n - 24)/6 if n
 *            odd : a(n) = (n^3 + 6*n^2 + 11*n - 30)/6 (Valid for all n ≥ 2,
 *            behaviour below the offset is unspecified.)
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t nn = N * N;                 /* N^2 */
    seq_t num;

    if ((n & 1ULL) == 0)              /* even n */
    {
        num = N * nn + 6 * nn + 14 * N - 24;
    }
    else                              /* odd n */
    {
        num = N * nn + 6 * nn + 11 * N - 30;
    }

    return num / 6;
}
