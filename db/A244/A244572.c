/*
 * A244572  a(n) = max(A244570(n), A244571(n)).
 * https://oeis.org/A244572
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(A244570(n), A244571(n)), where A244570(n) is the
 *            smallest prime p >= prime(n) with p*prime(n)+2 prime, and
 *            A244571(n) is the smallest prime p >= prime(n) with p*prime(n)-2
 *            prime. Computed by direct search
 * Range:     u8 n <= 29, u16 n <= 6494, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9423 (2026-11-07)
 * Helpers:   is_prime_u64, next_prime, nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"
#include "../utils/nth_prime.h"

static uint64_t A244572_search(uint64_t q, int sign)
{
    uint64_t p = q;
    for (;;) {
        uint64_t v = p * q;
        v = (sign > 0) ? v + 2 : v - 2;
        if (is_prime_u64(v)) return p;
        p = next_prime(p + 1);
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    uint64_t q = nth_prime(n);
    uint64_t x = A244572_search(q, 1);
    uint64_t y = A244572_search(q, -1);
    return (seq_t)(x > y ? x : y);
}
