/*
 * A339167  Rounded value of 1/(x[n] + n), where x[n] is the n-th negative
 *          solution to Gamma(x) = x^2.
 * https://oeis.org/A339167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * n! * n^2 - 3 (valid for n >= 10) For 1 <= n < 10
 *            the values are hard‑coded: a(1) = -2, a(2) = 5, a(3) = -56, a(4)
 *            = 382, a(5) = -3002, a(6) = 25918, a(7) = -246962, a(8) =
 *            2580478, a(9) = -29393282
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 10, i64 n <= 18, i128 n <= 20,
 *            big any n
 * Helpers:   factorial, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"   // provides seq_t factorial(idx_t n)
#include "../utils/mul_small.h"   // provides seq_t mul_small(seq_t x, unsigned int m)

seq_t a(idx_t n)
{
    /* Handle out‑of‑bounds or offset cases */
    if (n == 0)
        return 0;

    /* Hard‑coded first nine terms */
    if (n <= 9) {
        static const seq_t first[10] = {
            0,              /* dummy for index 0 */
            -2,             /* a(1) */
             5,             /* a(2) */
            -56,            /* a(3) */
            382,            /* a(4) */
           -3002,           /* a(5) */
           25918,           /* a(6) */
          -246962,          /* a(7) */
          2580478,          /* a(8) */
         -29393282          /* a(9) */
        };
        return first[(int)n];
    }

    /* Compute a(n) = (-1)^n * n! * n^2 - 3 for n >= 10 */
    seq_t fact = factorial(n);          /* n! */
    idx_t nsq_ull = (idx_t)n * (idx_t)n; /* n^2 fits in uint64_t for n <= 10000 */

    /* Multiply n! by n^2 */
    seq_t prod = mul_small(fact, (unsigned int)nsq_ull);

    /* Apply sign and subtract 3 */
    if (n % 2 == 0) {          /* even n  -> + sign */
        return prod - 3;
    } else {                    /* odd n   -> - sign */
        return -prod - 3;
    }
}
