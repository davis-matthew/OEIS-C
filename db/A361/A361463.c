/*
 * A361463  a(n) = 1 if A135506(n) == 3 (mod 4), otherwise 0.
 * https://oeis.org/A361463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A135506(n) == 3 (mod 4), otherwise 0; A135506(n) =
 *            x(n+1)/x(n) - 1, x(1) = 1, x(k) = x(k-1) + lcm(x(k-1),k)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 131 terms through n = 9679 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static inline uint32_t A361463_gcd32(uint32_t a, uint32_t b)
{
    while (b) {
        uint32_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static uint64_t A135506_small(uint32_t n)
{
    if (n < 1) return 0;

    uint32_t *mod = (uint32_t*)malloc(((size_t)n + 2) * sizeof(uint32_t));
    if (!mod) return 0;

    for (uint32_t k = 2; k <= n + 1; ++k) {
        mod[k] = 1 % k;
    }

    for (uint32_t j = 2; j <= n; ++j) {
        uint32_t g = A361463_gcd32(mod[j], j);
        uint32_t h = j / g;
        for (uint32_t k = 2; k <= n + 1; ++k) {
            uint32_t factor = 1 + h;
            if (factor >= k) factor %= k;
            mod[k] = (uint32_t)(((uint64_t)mod[k] * factor) % k);
        }
    }

    uint32_t gfinal = A361463_gcd32(mod[n + 1], n + 1);
    uint64_t result = (uint64_t)(n + 1) / gfinal;

    free(mod);
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t v = A135506_small((uint32_t)n);
    return (seq_t)((v % 4 == 3) ? 1 : 0);
}
