/*
 * A354467  Positive integers whose prime factors are congruent to 1 (mod 12).
 * https://oeis.org/A354467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer whose prime factors are all ≡ 1
 *            (mod 12)
 * Range:     u8 n <= 13, u16 n <= 2011, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9952 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* ------------------------------------------------------------------ */
static bool factors_ok(uint64_t x)
{
    if (x == 1) return true;

    uint64_t tmp = x;

    /* trial division – efficient enough for the size of numbers we
       need to generate (≈10⁷ for n≈10⁴). */
    for (uint64_t d = 2; d * d <= tmp; ++d) {
        if (tmp % d == 0) {
            if (d % 12 != 1) return false;
            while (tmp % d == 0) tmp /= d;
        }
    }
    if (tmp > 1 && tmp % 12 != 1) return false;

    return true;
}

/* ------------------------------------------------------------------ */
/* Helper: test whether all prime factors of x are ≡ 1 (mod 12)         */

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_candidate = 2;   /* next integer to test */

    if (!cache) {                     /* first call – initialise cache */
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) abort();
        cache[cache_len++] = (seq_t)1;  /* a(1) = 1 */
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        if (factors_ok(next_candidate)) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[(size_t)n - 1];
}
