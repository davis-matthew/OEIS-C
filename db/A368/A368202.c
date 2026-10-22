/*
 * A368202  Least k such that 6*n*k+1 is a prime.
 * https://oeis.org/A368202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least positive integer k such that 6*n*k + 1 is prime
 *            (OEIS A368202)
 * Range:     u8 n <= 13, u16 n <= 2705, u32 n <= 2705, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* If called with n=0 (outside the defined offset) return 0 to avoid
       infinite loop, as the behaviour below the offset does not matter. */
    if (n == 0) {
        return 0;
    }

    /* Search for the smallest k >= 1 for which 6*n*k + 1 is prime. */
    seq_t k = 1;
    const seq_t six = (seq_t)6;
    const seq_t one = (seq_t)1;

    while (1) {
        seq_t val = six * (seq_t)n * k + one;
        if (is_prime_big(val))
            return k;
        ++k;
    }
}
