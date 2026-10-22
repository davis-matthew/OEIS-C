/*
 * A358279  a(n) = Sum_{d|n} (d-1)! * d^(n/d).
 * https://oeis.org/A358279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} (d-1)! * d^(n/d)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* offset starts at 1; for n < 1 behaviour is undefined but guard */
    if (n == 0) return 0;

    seq_t total = 0;

    /* iterate over divisors by checking i up to sqrt(n) */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* first divisor d = i */
            idx_t d1 = i;
            idx_t exp1 = n / d1;
            seq_t term1 = factorial(d1 - 1) * big_pow((seq_t)d1, (seq_t)exp1);
            total += term1;

            /* second divisor d = n/i (if distinct) */
            idx_t d2 = n / i;
            if (d2 != d1) {
                idx_t exp2 = n / d2;
                seq_t term2 = factorial(d2 - 1) * big_pow((seq_t)d2, (seq_t)exp2);
                total += term2;
            }
        }
    }

    return total;
}
