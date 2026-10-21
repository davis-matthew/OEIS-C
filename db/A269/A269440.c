/*
 * A269440  Alternating sum of 9-gonal (or decagonal) pyramidal numbers.
 * https://oeis.org/A269440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ((-1)^n*(2*n-1)*(14*n^2 + 34*n + 15) + 15) / 48
 * Range:     i8 n <= 1, i16 n <= 9, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the unsigned index to seq_t for arithmetic */
    seq_t nn = (seq_t)n;

    /* sign: +1 for even n, -1 for odd n */
    seq_t sign = ((n & 1ULL) == 0) ? 1 : -1;

    /* components of the polynomial */
    seq_t term1 = 2 * nn - 1;                         /* 2*n - 1  */
    seq_t poly  = 14 * nn * nn + 34 * nn + 15;        /* 14*n^2+34*n+15 */

    /* numerator: sign * (2*n-1)*(14*n^2+34*n+15) + 15 */
    seq_t numer = sign * term1 * poly + 15;

    /* division by 48; exact integer result */
    return numer / 48;
}
