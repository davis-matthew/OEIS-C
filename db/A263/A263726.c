/*
 * A263726  Least prime p such that p^2 + A263977(n)^2 is prime.
 * https://oeis.org/A263726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that p^2 + k^2 is prime, where k =
 *            A263977(n); computed by search
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Validated: partial, pass, 89 terms through n = 89 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

/* k = A263977(n): even k always qualify; odd k qualify iff k^2+4 is prime */
static uint64_t A263726_k(idx_t n)
{
    uint64_t count = 0;
    uint64_t k = 1;
    for (;;) {
        bool ok;
        if ((k & 1ULL) == 0ULL) ok = true;
        else ok = is_prime_u64(k * k + 4ULL);
        if (ok) {
            ++count;
            if (count == (uint64_t)n) return k;
        }
        ++k;
    }
}

seq_t a(idx_t n)
{
    uint64_t k = A263726_k(n);
    uint64_t k2 = k * k;
    uint64_t p = 2;
    for (;;) {
        if (is_prime_u64(p) && is_prime_u64(p * p + k2))
            return (seq_t)p;
        p += (p == 2) ? 1 : 2;
    }
}
