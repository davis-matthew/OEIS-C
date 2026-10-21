/*
 * A281794  The largest prime factor of (1+n^2)*(1+n^3).
 * https://oeis.org/A281794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max( largest_prime_factor(n^2 + 1), *
 *            largest_prime_factor(n^3 + 1) )
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    uint64_t Nu = (uint64_t)n;
    uint64_t n2 = Nu * Nu + 1;          /* n^2 + 1 */
    uint64_t n3 = Nu * Nu * Nu + 1;     /* n^3 + 1 */
    uint64_t lpf1 = largest_prime_factor(n2);
    uint64_t lpf2 = largest_prime_factor(n3);
    uint64_t best = (lpf1 > lpf2) ? lpf1 : lpf2;
    return (seq_t)best;
}
