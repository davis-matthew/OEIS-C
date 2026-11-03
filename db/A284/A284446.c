/*
 * A284446  a(n) = Sum_{d|n, d == 5 (mod 7)} d.
 * https://oeis.org/A284446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d == 5 (mod 7)} d
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 98 terms through n = 9514 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;     /* cache[0] unused, cache[1] = a(1) ... */
static idx_t cache_n = 0;       /* maximum n for which cache is valid */

/* Rebuild cache up to new_n */
static void recompute_cache(idx_t new_n)
{
    /* Allocate new array */
    seq_t *new_cache = seq_alloc((size_t)new_n + 1);
    for (idx_t i = 0; i <= new_n; ++i) {
        new_cache[i] = 0;
    }

    /* Sieve: add each divisor d ≡ 5 mod 7 to all multiples */
    for (idx_t d = 5; d <= new_n; d += 7) {
        seq_t sd = (seq_t)d;
        for (idx_t m = d; m <= new_n; m += d) {
            new_cache[m] += sd;
        }
    }

    /* Replace old cache (leak is fine for small sizes) */
    cache = new_cache;
    cache_n = new_n;
}

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0;
    }
    if (n > cache_n) {
        recompute_cache(n);
    }
    return cache[n];
}
