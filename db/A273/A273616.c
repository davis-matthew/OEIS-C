/*
 * A273616  Number of ordered ways to write n as x^2 + y^2 + z^2 + w^2 with
 *          (3*x^2+13*y^2)*z a square, where x,y,z,w are nonnegative integers.
 * https://oeis.org/A273616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the ordered quadruples (x,y,z,w) of non‑negative
 *            integers * with x^2 + y^2 + z^2 + w^2 = n and (3*x^2 + 13*y^2) *
 *            z a perfect square
 * Range:     u8 n <= 7228, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* integer square root of a 32‑bit value (rounded down) */
static inline unsigned int isqrt32(uint32_t v)
{
    unsigned int r = (unsigned int)(sqrt((double)v) + 0.5);
    while ((uint64_t)r * r > v) --r;
    while ((uint64_t)(r + 1) * (r + 1) <= v) ++r;
    return r;
}

/* test whether a 32‑bit value is a perfect square */
static inline bool is_square32(uint32_t v)
{
    unsigned int r = isqrt32(v);
    return (uint64_t)r * r == v;
}

/* pre‑compute a(0)..a(max_n) into the array 'cnt' */
static void precompute(seq_t *cnt, size_t max_n)
{
    /* zero the whole array first */
    for (size_t i = 0; i <= max_n; ++i) cnt[i] = 0;

    for (size_t x = 0; x * x <= max_n; ++x) {
        uint32_t x2 = (uint32_t)x * x;
        for (size_t y = 0; y * y <= max_n - x2; ++y) {
            uint32_t y2   = (uint32_t)y * y;
            uint32_t xy   = x2 + y2;
            uint32_t cond = 3 * x2 + 13 * y2;

            for (size_t z = 0; z * z <= max_n - xy; ++z) {
                uint32_t z2 = (uint32_t)z * z;
                uint32_t base = xy + z2;

                /* test condition: (3*x^2+13*y^2)*z is a square (or zero) */
                uint32_t prod = cond * (uint32_t)z;   /* fits in 32 bits for max_n=10000 */
                if (prod == 0 || is_square32(prod)) {
                    uint32_t rem = (uint32_t)max_n - base;
                    for (size_t w = 0; w * w <= rem; ++w) {
                        uint32_t w2 = (uint32_t)w * w;
                        uint32_t n  = base + w2;
                        ++cnt[n];
                    }
                }
            }
        }
    }
}

/* public interface required by the test harness */
seq_t a(idx_t n)
{
    /* dynamic cache: grow on demand and recompute when the limit increases */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) { /* fallback on failure, though unlikely */ return 0; }
        cache = new_cache;
        /* recompute all counts up to the new maximum */
        precompute(cache, new_size - 1);
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
