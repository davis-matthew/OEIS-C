/*
 * A321881  Numbers whose sum and product of digits are cubes.
 * https://oeis.org/A321881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   generate numbers whose sum and product of digits are perfect
 *            cubes, ordered increasingly
 * Range:     u8 n <= 9, u16 n <= 769, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product, digit_sum, floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
/* utility headers – compile‑time will provide the correct definitions */
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/floor_cube_root.h"

static seq_t *cache = NULL;      /* cached sequence values (0‑based) */
static size_t cache_size = 0;    /* number of cached terms */
static size_t cache_cap   = 0;   /* allocated capacity of cache */

/* Return true if x is a perfect cube. */
static inline bool is_cube(seq_t x)
{
    seq_t c = floor_cube_root(x);
    return c * c * c == x;
}

/* Ensure the cache has room for at least one more element. */
static void ensure_cache_capacity(void)
{
    if (cache_cap == 0) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_size >= cache_cap) {
        cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* Return the n‑th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* behaviour below the offset is undefined */

    /* Prime the cache with a(1) = 0 if this is the first call. */
    if (cache_size == 0) {
        ensure_cache_capacity();
        cache[0] = 0; /* a(1) = 0 */
        cache_size = 1;
    }

    /* Grow the cache until it contains at least n elements. */
    while (cache_size < (size_t)n) {
        seq_t last = cache[cache_size - 1];
        seq_t cand = last + 1;

        /* Search forward for the next qualifying number. */
        while (true) {
            seq_t s = digit_sum(cand);
            seq_t p = digit_product(cand);

            if (is_cube(s) && is_cube(p)) {
                ensure_cache_capacity();
                cache[cache_size++] = cand;
                break;
            }
            cand = cand + 1;
        }
    }

    return cache[n - 1];
}
