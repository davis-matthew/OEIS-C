/*
 * A343936  Number of ways to choose a multiset of n divisors of n - 1.
 * https://oeis.org/A343936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n + tau(n) - 2, tau(n) - 1) where tau(n) is the
 *            number of divisors of n (A000005)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   binomial, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    /* Number of divisors of n */
    idx_t tau = num_divisors(n);

    /* Compute the top and bottom of the binomial coefficient.
       For n >= 1, tau >= 1, so no underflow occurs. */
    idx_t top = n + tau - 2;
    idx_t bot = tau - 1;

    return binomial(top, bot);
}
