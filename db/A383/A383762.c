/*
 * A383762  The number of unitary divisors of n that are exponentially
 *          squarefree numbers.
 * https://oeis.org/A383762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (2 if e is squarefree else 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts unitary divisors of n that are exponentially squarefree. */
#include <stdbool.h>
#include "../utils/is_squarefree_exp.h"

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined, but return 0 to avoid UB */
        return 0;

    seq_t result = 1;
    idx_t m = n;
    idx_t p = 2;

    /* trial division */
    while (p <= m / p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            if (is_squarefree_exp(exp))
                result *= 2;
        }
        ++p;
    }

    /* if a prime factor > sqrt(n) remains */
    if (m > 1) {
        /* exponent is 1, which is squarefree */
        result *= 2;
    }

    return result;
}
