/*
 * A319116  Signs of the Maclaurin coefficients of 1/(exp(x) + Pi/2).
 * https://oeis.org/A319116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sign( cos( (n+1) * arctan( π / ln(π/2) ) ) )
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: π is approximated as a long double constant.  */

/* #include required for atanl, cosl, logl */
#include <math.h>

/* Compute the sign of the Maclaurin coefficients of 1/(exp(x) + π/2). */
seq_t a(idx_t n)
{
    /* High‑precision π constant */
    static const long double pi = 3.1415926535897932384626433832795028841971693993751L;

    /* Compute the fixed irrational angle θ = atan(π / ln(π/2)) */
    static const long double theta = atanl(pi / logl(pi / 2.0L));

    /* Multiply by (n+1) and evaluate cosine */
    long double v = cosl((long double)(n + 1) * theta);

    /* Return +1 or -1 according to the sign of v */
    return (v > 0.0L) ? (seq_t)1 : (seq_t)-1;
}
