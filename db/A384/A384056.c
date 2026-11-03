/*
 * A384056  The number of integers k from 1 to n such that the greatest
 *          divisor of k that is a unitary divisor of n is a power of 2.
 * https://oeis.org/A384056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p^e||n} f(p^e) where f(2^e)=2^e, f(p^e)=p^e-1
 *            for odd primes
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9484 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;   /* a(1) = 1 */

    seq_t result = (seq_t)1;
    idx_t m = n;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (p == 2) {
                /* 2^e */
                result *= ipow_u64_idx(2, e);
            } else {
                /* p^e - 1 */
                seq_t val = ipow_u64_idx(p, e);
                val -= (seq_t)1;
                result *= val;
            }
        }
    }

    if (m > 1) {          /* remaining prime factor */
        if (m == 2) {
            result *= (seq_t)2;
        } else {
            seq_t val = ipow_u64_idx(m, 1);
            val -= (seq_t)1;
            result *= val;
        }
    }

    return result;
}
