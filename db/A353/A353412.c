/*
 * A353412  The odd part of hybrid shift: a(n) = A000265(A252463(n)).
 * https://oeis.org/A353412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n) = A000265(n), a(2n-1) = odd part of prod prevprime(p)^e
 *            over p^e || 2n-1 (a(1)=1)
 * Range:     u8 n <= 262, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 104 terms through n = 8899 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool oddpart_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t oddpart_prevprime(uint64_t p)
{
    uint64_t q = p - 1;
    while (q >= 2 && !oddpart_isprime(q)) --q;
    return q;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if ((n & 1u) == 0u) {
        uint64_t h = (uint64_t)(n >> 1);
        while ((h & 1u) == 0u) h >>= 1;
        return (seq_t)h;
    }
    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; ++e; }
            if (p == 3) continue; /* prevprime(3) = 2: contributes only powers of 2 */
            seq_t base = (seq_t)oddpart_prevprime(p);
            for (unsigned i = 0; i < e; ++i) result *= base;
        }
    }
    if (m > 1 && m != 3) {
        result *= (seq_t)oddpart_prevprime(m);
    }
    return result;
}
