/*
 * A250123  Coordination sequence of point of type 3.3.4.3.4 in 4-uniform
 *          tiling {3.3.4.3.4; 3.3.4.12; 3.3.12.4; 3.4.3.12}.
 * https://oeis.org/A250123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-5)+a(n-7)-a(n-12) for n>=17. Initial values (n =
 *            0..17): 1,5,8,8,11,17,25,27,24,30,38,46,47,44,46,50,64,68
 * Range:     u8 n <= 70, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init_terms[] = {
    1, 5, 8, 8, 11, 17, 25, 27,
    24, 30, 38, 46, 47, 44, 46, 50,
    64, 68
};
static const size_t init_len = sizeof(init_terms) / sizeof(init_terms[0]);

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Return pre‑computed initial value if requested. */
    if (n < init_len) {
        return init_terms[(size_t)n];
    }

    /* Ensure the cache is large enough to hold the required terms. */
    if (!cache || cache_size < init_len) {
        cache_size = init_len;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (size_t i = 0; i < init_len; ++i) {
            cache[i] = init_terms[i];
        }
    }

    /* Extend the cache up to n (inclusive). */
    if ((size_t)n >= cache_size) {
        size_t new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            /* The first 18 terms are already in init_terms. */
            if (i < init_len) {
                cache[i] = init_terms[i];
            } else {
                /* Recurrence a(n) = a(n-5)+a(n-7)-a(n-12) for n>=17. */
                cache[i] = cache[i - 5] + cache[i - 7] - cache[i - 12];
            }
        }
        cache_size = new_cap;
    }

    return cache[(size_t)n];
}
