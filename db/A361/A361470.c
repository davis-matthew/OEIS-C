/*
 * A361470  a(n) = gcd(n+1, A135504(n)).
 * https://oeis.org/A361470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n+1, A135504(n)), where A135504(1)=1, A135504(k) =
 *            A135504(k-1) + lcm(A135504(k-1), k); computed by tracking x_k
 *            mod m for all m <= n+1
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 8856 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint32_t A361470_gcd32(uint32_t a, uint32_t b)
{
    while (b) {
        uint32_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint32_t N = (uint32_t)n;
    uint32_t *mod = (uint32_t *)malloc(((size_t)N + 2) * sizeof(uint32_t));
    if (!mod) return (seq_t)0;

    for (uint32_t k = 2; k <= N + 1; ++k)
        mod[k] = 1 % k;

    for (uint32_t j = 2; j <= N; ++j) {
        uint32_t g = A361470_gcd32(mod[j], j);
        uint32_t h = 1 + j / g;
        for (uint32_t k = j; k <= N + 1; ++k) {
            uint32_t f = h;
            if (f >= k) f %= k;
            mod[k] = (uint32_t)(((uint64_t)mod[k] * f) % k);
        }
    }

    uint32_t r = A361470_gcd32(mod[N + 1], N + 1);
    free(mod);
    return (seq_t)r;
}
