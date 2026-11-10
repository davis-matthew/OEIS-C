/*
 * A371098  a(n) = gcd(2n+1, A276086(2n+1)), where A276086 is the primorial
 *          base exp-function.
 * https://oeis.org/A371098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(2n+1, A276086(2n+1)); computed as the product over
 *            primes p_k of p_k^min(v_{p_k}(2n+1), d_k), where d_k are the
 *            primorial-base digits of 2n+1
 * Range:     u8 n <= 170, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 117 terms through n = 9433 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool a371098_isprime(uint64_t p)
{
    if (p < 2) return false;
    for (uint64_t d = 2; d * d <= p; ++d)
        if (p % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n * 2 + 1;
    uint64_t r = m;
    seq_t result = (seq_t)1;
    for (uint64_t p = 2; r > 0; ++p) {
        if (!a371098_isprime(p)) continue;
        uint64_t d = r % p;
        r /= p;
        uint64_t t = m;
        for (uint64_t i = 0; i < d && t % p == 0; ++i) {
            t /= p;
            result *= (seq_t)p;
        }
    }
    return result;
}
