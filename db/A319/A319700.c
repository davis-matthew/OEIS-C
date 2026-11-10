/*
 * A319700  a(n) = A051953(A252463(n)).
 * https://oeis.org/A319700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A051953(A252463(n)), where A051953(m) = m - phi(m);
 *            A252463(1)=1, A252463(2k)=k, A252463(odd n) = prod
 *            prev_prime(p)^e over n = prod p^e
 * Range:     u8 n <= 719, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9987 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A319700_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x < 4) return true;
    if ((x & 1u) == 0u) return false;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A319700_prevprime(uint64_t p)
{
    uint64_t q = p - 1;
    while (!A319700_isprime(q)) --q;
    return q;
}

static uint64_t A319700_hybrid(uint64_t n)
{
    if (n == 1) return 1;
    if ((n & 1u) == 0u) return n >> 1;
    uint64_t m = n, result = 1;
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t base = A319700_prevprime(p);
            while (m % p == 0) {
                m /= p;
                result *= base;
            }
        }
    }
    if (m > 1) result *= A319700_prevprime(m);
    return result;
}

static uint64_t A319700_phi(uint64_t m)
{
    uint64_t result = m;
    for (uint64_t d = 2; d <= m / d; ++d) {
        if (m % d == 0) {
            while (m % d == 0) m /= d;
            result -= result / d;
        }
    }
    if (m > 1) result -= result / m;
    return result;
}

seq_t a(idx_t n)
{
    uint64_t h = A319700_hybrid((uint64_t)n);
    return (seq_t)(h - A319700_phi(h));
}
