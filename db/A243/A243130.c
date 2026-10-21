/*
 * A243130  a(n) = 1024*n^10 - 2304*n^8 + 1792*n^6 - 560*n^4 + 60*n^2 - 1.
 * https://oeis.org/A243130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1024*n^10 - 2304*n^8 + 1792*n^6 - 560*n^4 + 60*n^2 - 1
 * Range:     i8 n <= 1, i16 n <= 1, i32 n <= 4, i64 n <= 39, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute m = n^2 once and use Horner's method for the 5‑degree polynomial in m */
    seq_t nn = (seq_t)n;
    seq_t m  = nn * nn;          /* n^2                         */

    seq_t val = (seq_t)1024;      /* start with the leading coeff */

    val = val * m - (seq_t)2304;  /* (1024*m - 2304)              */
    val = val * m + (seq_t)1792;  /* ((1024*m - 2304)*m + 1792)   */
    val = val * m - (seq_t)560;   /* (((1024*m - 2304)*m + 1792)*m - 560) */
    val = val * m + (seq_t)60;    /* ((((1024*m - 2304)*m + 1792)*m - 560)*m + 60) */
    val = val * m - (seq_t)1;     /* (((((1024*m - 2304)*m + 1792)*m - 560)*m + 60)*m - 1) */

    return val;
}
