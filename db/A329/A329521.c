/*
 * A329521  The sixth moments of the alternated squared binomial coefficients;
 *          a(n) = Sum_{m=0..n} (-1)^m*m^6*binomial(n, m)^2.
 * https://oeis.org/A329521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{m=0..n} (-1)^m * m^6 * binomial(n, m)^2 Equivalent
 *            closed forms: if n is even: a(n) = (-1)^((n+2)/2) * binomial(n,
 *            n/2) * ( n^3*(n+1)*(3n-1) / 4 ) if n is odd: a(n) =
 *            (-1)^((n-1)/2) * binomial(n, (n+1)/2) * (
 *            n^2*(n+1)*(n^3+n^2-9n+3) / 8 ) The binomial function is provided
 *            by the util...
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 14, i64 n <= 38, i128 n <= 94,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Computes the n-th term of OEIS sequence A329521 */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* handle trivial case n = 0 explicitly (formula gives 0 anyway) */
    if (n == 0)
        return 0;

    seq_t sign;
    seq_t binom_val;
    seq_t part;
    seq_t result;

    if ((n & 1) == 0) {  /* n is even */
        idx_t exp = (n + 2) / 2;
        sign = (exp & 1) ? -1 : 1;

        binom_val = binomial(n, n / 2);

        /* numerator = n^3 * (n+1) * (3n-1) */
        seq_t num = nn * nn * nn * (nn + 1) * (3 * nn - 1);
        /* divide by 4 – guaranteed to be integer */
        part = num / 4;

        result = sign * binom_val * part;
    } else {            /* n is odd */
        idx_t exp = (n - 1) / 2;
        sign = (exp & 1) ? -1 : 1;

        binom_val = binomial(n, (n + 1) / 2);

        /* term inside the last parentheses */
        seq_t inner = nn * nn * nn + nn * nn - 9 * nn + 3;
        /* numerator = n^2 * (n+1) * inner */
        seq_t num = nn * nn * (nn + 1) * inner;
        /* divide by 8 – guaranteed to be integer */
        part = num / 8;

        result = sign * binom_val * part;
    }

    return result;
}
