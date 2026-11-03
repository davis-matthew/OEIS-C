/*
 * A329760  Primes without {2, 7} as digits.
 * https://oeis.org/A329760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A329760 = primes whose decimal representation contains neither 2
 *            nor 7
 * Range:     u8 n <= 26, u16 n <= 2158, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 71 terms through n = 9991 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

/* Return true if the decimal representation of x contains a 2 or a 7. */
static inline bool bad_digit_present(uint64_t x)
{
    while (x) {
        uint8_t d = x % 10;
        if (d == 2 || d == 7)
            return true;
        x /= 10;
    }
    return false;
}

static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_capacity = 0;    /* allocated capacity of cache */
static size_t cache_size = 0;        /* number of entries currently stored */

seq_t a(idx_t n)
{
    if (n == 0)                 /* the sequence is 1‑based; n==0 is unused */
        return 0;

    /* Ensure the cache has room for at least n elements. */
    ensure_cache(n, &cache, &cache_capacity);

    /* Generate terms until we have n elements in the cache. */
    while (cache_size < (size_t)n) {
        /* Start searching from the next integer after the last stored prime. */
        uint64_t cand = (cache_size == 0) ? 2
                                          : (uint64_t)(cache[cache_size - 1]) + 1;

        /* Find the next prime that avoids digits 2 and 7. */
        for (;;) {
            if (bad_digit_present(cand)) {
                ++cand;
                continue;
            }
            if (is_prime((idx_t)cand)) {
                cache[cache_size++] = (seq_t)cand;
                break;
            }
            ++cand;
        }
    }

    return cache[n - 1];
}
