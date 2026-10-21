/*
 * A248158  Expansion of (1 - 2*x^2)/(1 + x)^3. Second column of Riordan
 *          triangle A248156.
 * https://oeis.org/A248158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * (4*(2*n + 1) - (n+1)*(n+2)) / 2
 * Range:     i8 n <= 14, i16 n <= 205, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*             = (-1)^n * (-n^2 + 5*n + 2) / 2  */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t num = - (nseq * nseq) + 5 * nseq + 2;   /* numerator */
    seq_t res = num / 2;                          /* division by 2 */
    if (n & 1)                                    /* apply sign */
        res = -res;
    return res;
}
