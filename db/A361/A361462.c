/*
 * A361462  a(n) = A135506(n) mod 4.
 * https://oeis.org/A361462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A135506(n) mod 4, where A135506(n) = x(n+1)/x(n) - 1,
 *            x(1) = 1, x(k) = x(k-1) + lcm(x(k-1), k)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 136 terms through n = 9824 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static inline uint32_t A135506_gcd32(uint32_t a, uint32_t b)
{
    while (b) {
        uint32_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static seq_t A135506_a(idx_t n)
{
    if (n < 1) return 0;

    uint32_t *mod = (uint32_t*)malloc((size_t)(n + 2) * sizeof(uint32_t));
    if (!mod) return 0;

    for (uint32_t k = 2; k <= (uint32_t)(n + 1); ++k) {
        mod[k] = 1 % k;
    }

    for (uint32_t j = 2; j <= (uint32_t)n; ++j) {
        uint32_t g = A135506_gcd32(mod[j], j);
        uint32_t h = j / g;
        uint32_t factor;

        for (uint32_t k = 2; k <= (uint32_t)(n + 1); ++k) {
            factor = 1 + h;
            if (factor >= k) factor %= k;
            mod[k] = (uint32_t)(((uint64_t)mod[k] * factor) % k);
        }
    }

    uint32_t gfinal = A135506_gcd32(mod[n + 1], (uint32_t)(n + 1));
    seq_t result = ((seq_t)(n + 1)) / (seq_t)gfinal;

    free(mod);
    return result;
}

seq_t a(idx_t n)
{
    return A135506_a(n) % (seq_t)4;
}
