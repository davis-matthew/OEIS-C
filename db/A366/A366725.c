/*
 * A366725  Sum of odd indices of distinct prime factors of n.
 * https://oeis.org/A366725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of odd indices of distinct prime factors of n (OEIS
 *            A366725)
 * Range:     u8 n <= 1620, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9367 (2026-11-01)
 * Helpers:   prime_pi, smallest_prime_factor_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/prime_pi.h"
#include "../utils/smallest_prime_factor_idx.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t sum = 0;
    uint64_t temp = (uint64_t)n;
    uint64_t last_p = 0;

    while (temp > 1)
    {
        uint64_t p = smallest_prime_factor_idx(temp);          /* smallest prime factor */
        if (p != last_p)                 /* distinct prime factor */
        {
            seq_t idx = prime_pi(p);     /* prime index of p */
            if (idx & 1)                 /* odd index only */
                sum += idx;
            last_p = p;
        }
        while (temp % p == 0)
            temp /= p;
    }

    return sum;
}
