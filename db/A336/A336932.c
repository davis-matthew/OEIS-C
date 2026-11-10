/*
 * A336932  The 2-adic valuation of A003973(n), the sum of divisors of prime
 *          shifted n.
 * https://oeis.org/A336932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is additive with a(p^(2e)) = 0 and a(p^(2e-1)) =
 *            A007814(1+q) + A007814(e), where q = nextPrime(p)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9395 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static unsigned A336932_v2(uint64_t x) {
    unsigned c = 0;
    while (x && (x & 1) == 0) { x >>= 1; c++; }
    return c;
}

static unsigned A336932_term(uint64_t p, uint64_t k) {
    if ((k & 1) == 0) return 0;
    uint64_t q = next_prime(p);
    return A336932_v2(q + 1) + A336932_v2((k + 1) / 2);
}

seq_t a(idx_t n) {
    uint64_t m = n;
    seq_t result = 0;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t k = 0;
            while (m % p == 0) { m /= p; k++; }
            result += (seq_t)A336932_term(p, k);
        }
    }
    if (m > 1) result += (seq_t)A336932_term(m, 1);
    return result;
}
