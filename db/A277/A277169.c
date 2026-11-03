/*
 * A277169  Product of squares of proper divisors of n.
 * https://oeis.org/A277169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^(sigma_0(n)-2)
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 35, u64 n <= 89, u128 n <= 239,
 *            big any n
 * Validated: standard, pass, 66 terms through n = 9640 (2026-11-01)
 * Helpers:   ipow, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where sigma_0(n) is the number of positive divisors of n,
    so the product of squares of proper divisors equals n raised to
    the power (tau(n)-2).                                   */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    if (n <= 1u)              /* a(1) = 1 by definition */
        return 1;

    idx_t tau = num_divisors((uint64_t)n);
    if (tau <= 2u)            /* prime numbers (tau==2) give 1 */
        return 1;

    seq_t base = (seq_t)n;
    seq_t exp  = (seq_t)(tau - 2u);

    return ipow(base, exp);
}
