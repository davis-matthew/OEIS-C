/*
 * A302581  a(n) = n! * [x^n] -exp(-n*x)*log(1 - x).
 * https://oeis.org/A302581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 19,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=1..n} (-n)^(n-k) * (k-1)! * binomial(n,k)
 * Indexed so that a(0) = 0.
 */
#include "../utils/factorial.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    seq_t result = 0;          /* Accumulator for the sum                  */
    seq_t pow = 1;             /* Holds n^(n-k) for the current k          */
    const seq_t n_seq = (seq_t)n;  /* n as seq_t, used for multiplication   */

    for (idx_t k = n; k > 0; --k) {
        /* term = pow * (k-1)! * binomial(n,k) */
        seq_t term = pow * factorial(k - 1);
        term = term * binomial(n, k);

        /* apply sign: (-n)^(n-k) = (-1)^(n-k) * n^(n-k) */
        if ((n - k) & 1U) {
            term = -term;     /* flip the sign if exponent is odd        */
        }

        result += term;

        /* Update pow for next iteration (k -> k-1):
         * pow(new) = pow(old) * n
         */
        if (k > 1) {
            pow = pow * n_seq;
        }
    }

    return result;
}
