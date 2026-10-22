/*
 * A363615  Expansion of Sum_{k>0} x^(3*k)/(1+x^k)^3.
 * https://oeis.org/A363615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = - Σ_{d|n} (-1)^d * binomial(d-1, 2)
 * Range:     i8 n <= 15, i16 n <= 215, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Computes a(n) according to the divisor-sum formula.
   n is 1-based: a(1) is the first element. */
seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1; return 0 for n < 1. */
    if (n < 1) return 0;

    seq_t sum = 0;

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* process divisor d1 */
            if (d1 >= 2) {
                seq_t binom = (seq_t)(d1 - 1) * (seq_t)(d1 - 2) / 2;
                seq_t sign  = (d1 & 1U) ? -1 : 1;   /* (-1)^d1 */
                sum += sign * binom;
            }

            /* if d1 != d2, process the paired divisor d2 */
            if (d1 != d2 && d2 >= 2) {
                seq_t binom = (seq_t)(d2 - 1) * (seq_t)(d2 - 2) / 2;
                seq_t sign  = (d2 & 1U) ? -1 : 1;   /* (-1)^d2 */
                sum += sign * binom;
            }
        }
    }

    return -sum;
}
