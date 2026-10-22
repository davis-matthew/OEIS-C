/*
 * A364044  Expansion of Sum_{k>0} x^(2*k) / (1 + x^(5*k)).
 * https://oeis.org/A364044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum over divisors d of n with d ≡ 2 (mod 5) of (-1)^d
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: iterate divisors up to sqrt(n) and accumulate the sign */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;

    if (n < 1)
        return sum;  /* undefined for n < 1, return 0 */

    /* iterate over all divisor pairs i and n/i */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* divisor i */
            if (i % 5 == 2) {
                sum += ((i & 1) == 0) ? (seq_t)1 : (seq_t)-1;
            }

            /* corresponding divisor j = n / i
               avoid double counting when i == j */
            idx_t j = n / i;
            if (j != i) {
                if (j % 5 == 2) {
                    sum += ((j & 1) == 0) ? (seq_t)1 : (seq_t)-1;
                }
            }
        }
    }

    return sum;
}
