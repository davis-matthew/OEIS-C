/*
 * A358845  Numbers k for which A276086(6*k) == 1 (mod 6), where A276086 is
 *          the primorial base exp-function.
 * https://oeis.org/A358845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 0 with A276086(6*k) == 1 (mod 6). 6k has
 *            primorial digits d0=d1=0, so A276086(6k) = prod_{i>=2}
 *            prime(i+1)^d_i; since primes >= 5 are +-1 mod 6, the product is
 *            1 mod 6 iff the sum of digits d_i at primes p == 5 (mod 6) is
 *            even. Found by scanning k upward
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool ok_prime_small(uint64_t p)
{
    if (p < 2) return false;
    for (uint64_t d = 2; d * d <= p; ++d)
        if (p % d == 0) return false;
    return true;
}

static bool good_k(uint64_t k)
{
    uint64_t m = 6 * k;
    uint64_t p = 2;
    uint64_t parity = 0;
    while (m > 0) {
        uint64_t d = m % p;
        m /= p;
        if (p % 6 == 5) parity += d;
        p++;
        while (!ok_prime_small(p)) p++;
    }
    return (parity & 1) == 0;
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t k = 0;
    for (;; ++k) {
        if (good_k(k)) {
            ++count;
            if (count == (uint64_t)n) return (seq_t)k;
        }
    }
}
