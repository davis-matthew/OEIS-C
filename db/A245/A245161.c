/*
 * A245161  Number of length n 0..6 arrays with new values introduced in order
 *          from both ends.
 * https://oeis.org/A245161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 19, u64 n <= 31, u128 n <= 54,
 *            big any n
 * Helpers:   rec_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A245161
 * Number of length n 0..6 arrays with new values introduced in order from both ends.
 *
 * Empirical linear recurrence (valid for n > 12):
 *   a(n) = 37*a(n-1) - 605*a(n-2) + 5765*a(n-3) - 35523*a(n-4)
 *        + 148371*a(n-5) - 427775*a(n-6) + 849335*a(n-7)
 *        - 1134976*a(n-8) + 969292*a(n-9) - 474720*a(n-10)
 *        + 100800*a(n-11)
 *
 * Initial values (a(1) … a(12)):
 *   1, 1, 2, 4, 9, 23, 65, 199, 654, 2296, 8569, 33825
 *
 * The implementation uses memoisation and the recurrence for n > 12.
 * It works for any integer type seq_t (≥ 8 bits) and idx_t
 * (≥ 64 bits), as required by the build system.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/rec_value.h"

/* --------- initial values ------------------------------------------------ */
static const seq_t init_vals[13] = {
    0,    /* a(0) – unused */
    1,    /* a(1) */
    1,    /* a(2) */
    2,    /* a(3) */
    4,    /* a(4) */
    9,    /* a(5) */
    23,   /* a(6) */
    65,   /* a(7) */
    199,  /* a(8) */
    654,  /* a(9) */
    2296, /* a(10) */
    8569, /* a(11) */
    33825 /* a(12) */
};

/* --------- cache management ------------------------------------------------ */
static seq_t *cache = NULL;          /* cache[0] unused */
static size_t cache_cap = 0;         /* current capacity (bytes) */
static idx_t cache_max = 0;          /* largest n computed */

/* enlarge cache to hold at least size n (1‑based) */
static void ensure_cache(idx_t n)
{
    if (cache_cap > (size_t)n) return; /* already enough */

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= (size_t)n) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;

    /* initialise the new slots to 0 (not strictly required) */
    for (size_t i = cache_cap / 2; i < cache_cap; ++i) cache[i] = 0;
}

/* --------- public function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n < 1 */

    /* return pre‑computed values directly for n <= 12 */
    if (n <= 12) return init_vals[(size_t)n];

    /* ensure cache covers n and compute missing values */
    if (cache_max < n) {
        ensure_cache(n);
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i <= 12) {
                cache[i] = init_vals[(size_t)i];
            } else {
                cache[i] = rec_value(cache, i);
            }
        }
        cache_max = n;
    }
    return cache[(size_t)n];
}
