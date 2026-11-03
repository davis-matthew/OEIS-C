/*
 * A369868  a(n) = Sum_{p|n, p prime} n^pi(n/p).
 * https://oeis.org/A369868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p|n, p prime} n^pi(n/p)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 33, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   ipow, prime_pi, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/prime_pi.h"
#include "../utils/smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;          /* a(1) = 0 */

    idx_t original = n;          /* keep the original n */
    seq_t sum = 0;
    uint64_t temp = (uint64_t)n;

    while (temp > 1) {
        /* smallest prime factor of the remaining part */
        uint64_t p = smallest_prime_factor(temp);

        /* compute π(original / p) and raise original to that power */
        idx_t arg = original / p;                 /* n/p */
        seq_t pi_val = prime_pi(arg);            /* π(n/p) */
        seq_t term = ipow((seq_t)original, pi_val);
        sum += term;

        /* remove all copies of this prime from temp */
        while (temp % p == 0)
            temp /= p;
    }

    return sum;
}
