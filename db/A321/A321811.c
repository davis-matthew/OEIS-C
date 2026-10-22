/*
 * A321811  Sum of 7th powers of odd divisors of n.
 * https://oeis.org/A321811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sigma_7(odd_part(n)) where odd_part(n) is n with all
 *            factors of 2 removed. sigma_7(m) = Σ_{d|m} d^7, multiplicative:
 *            for m = ∏p_i^e_i, sigma_7(m) = ∏ (p_i^(7*e_i+7)-1)/(p_i^7-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 24, u64 n <= 566,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    uint64_t odd = (uint64_t)n;
    while ((odd & 1ULL) == 0ULL)
        odd >>= 1U;
    return sigma_k(odd, 7);
}
