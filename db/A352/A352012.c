/*
 * A352012  a(n) = Sum_{p|n, p prime} (n-1)!/(p-1)!.
 * https://oeis.org/A352012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} (n-1)!/(p-1)!
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Provide the factorial helper from the utils library. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n < 2) {           /* a(1) = 0, a(0) is outside the range */
        return 0;
    }

    /* (n-1)! is needed for every prime factor of n.  Compute it once. */
    seq_t fact_n_minus1 = factorial(n - 1);

    seq_t sum = 0;
    uint64_t m = (uint64_t)n;         /* working copy for factorisation */
    uint64_t orig = m;                /* keep the original value of n */

    /* Enumerate all distinct prime divisors up to sqrt(n). */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            /* p is a distinct prime divisor. */
            seq_t fact_p_minus1 = factorial(p - 1);
            sum += fact_n_minus1 / fact_p_minus1;

            /* Remove all occurrences of this prime. */
            while (m % p == 0) {
                m /= p;
            }
        }
    }

    /* If a prime greater than sqrt(n) remains, it is the last distinct
       prime divisor.  Handle it separately. */
    if (m > 1) {
        if (m == orig) {            /* n itself is prime -> term = 1 */
            sum += 1;
        } else {
            seq_t fact_p_minus1 = factorial(m - 1);
            sum += fact_n_minus1 / fact_p_minus1;
        }
    }

    return sum;
}
