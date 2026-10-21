/*
 * A243145  Least positive number k such that n+k and n+k^2 are both prime.
 * https://oeis.org/A243145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least positive integer k such that n+k and n+k^2 are
 *            both prime
 * Range:     u8 n <= 490, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    for (idx_t k = 1; ; ++k) {
        idx_t cand1 = n + k;
        idx_t cand2 = n + k * k;
        if (is_prime(cand1) && is_prime(cand2))
            return (seq_t)k;
    }
}
