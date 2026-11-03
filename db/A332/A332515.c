/*
 * A332515  Numbers k such that phi(k) == 8 (mod 12), where phi is the Euler
 *          totient function (A000010).
 * https://oeis.org/A332515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 7194, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9139 (2026-11-03)
 * Helpers:   mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based computation of a(n) – nth smallest integer k with φ(k) ≡ 8 (mod 12) */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/totient.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache of discovered values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t last_tested = 0;   /* last integer tested */

    /* Ensure we have at least n values in the cache */
    while (cache_len < (size_t)n) {
        ++last_tested;                       /* next integer to test */
        seq_t phi_val = totient(last_tested);
        if ((phi_val % 12) == 8) {
            /* Append to cache */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_tested;
        }
    }

    return cache[(size_t)n - 1];
}
