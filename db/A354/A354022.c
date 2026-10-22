/*
 * A354022  a(n) = n! * Sum_{d|n} mu(n/d) / d!.
 * https://oeis.org/A354022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{d|n} mu(n/d) / d! (OEIS A354022)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   factorial, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factorial.h"
#include "../utils/mobius.h"

seq_t a(idx_t n) {
    seq_t fac_n = factorial(n);   /* compute n! */
    seq_t sum = 0;                /* running sum of terms */

    /* Enumerate all divisors d of n */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* process divisor d */
            idx_t dd = d;
            seq_t mu1 = mobius(n / dd);          /* mu(n/d) */
            if (mu1 != 0) {
                seq_t div1 = fac_n / factorial(dd); /* n! / d! */
                sum += mu1 * div1;
            }

            /* process paired divisor e = n/d if distinct */
            idx_t e = n / d;
            if (e != dd) {
                seq_t mu2 = mobius(n / e);          /* mu(n/e) = mu(d) */
                if (mu2 != 0) {
                    seq_t div2 = fac_n / factorial(e); /* n! / e! */
                    sum += mu2 * div2;
                }
            }
        }
    }

    return sum;
}
