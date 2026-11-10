/*
 * A385538  First differences of A184859.
 * https://oeis.org/A385538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A184859(n+1) - A184859(n); A184859 = primes among
 *            floor(k*(1+sqrt(5))/2 + 1/2), found by a linear search over k
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-10)
 * Helpers:   is_prime_u64, isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt_u128.h"

static uint64_t A385538_U(uint64_t k)
{
    unsigned __int128 prod = (unsigned __int128)5 * k * k;
    uint64_t s = (uint64_t)isqrt_u128(prod);
    return (k + s + 1) / 2;
}

seq_t a(idx_t n)
{
    uint64_t found = 0;
    uint64_t k = 1;
    uint64_t first = 0;
    for (;;) {
        uint64_t u = A385538_U(k);
        if (is_prime_u64(u)) {
            ++found;
            if (found == (uint64_t)n) first = u;
            else if (found == (uint64_t)n + 1) return (seq_t)(u - first);
        }
        ++k;
    }
}
