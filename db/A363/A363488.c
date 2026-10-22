/*
 * A363488  Even numbers whose prime factorization has at least as many 2's as
 *          non-2's.
 * https://oeis.org/A363488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   good_even
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A363488: a(n) is the n‑th even integer m such that
   2*v2(m) ≥ Ω(m) (the exponent of 2 is at least half the total
   count of prime factors). */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/good_even.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined, but safer */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t next_candidate = 0;

    while (cache_len < n) {
        if (next_candidate == 0ULL)
            next_candidate = 2ULL;
        uint64_t m = next_candidate;
        next_candidate += 2ULL;   /* next even number */

        if (good_even(m)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)m;
        }
    }
    return cache[n - 1];
}
