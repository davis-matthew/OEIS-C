/*
 * A398833  a(n) is the number of tilings of a 10 X n rectangle into
 *          L-pentominoes (no reflections, only rotations).
 * https://oeis.org/A398833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 50, u32 n <= 96, u64 n <= 187,
 *            u128 n <= 370, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A398833 – tilings of a 10×n rectangle by L‑pentominoes
   Formula: a(n) = a(n-2) + a(n-5) + a(n-10),
   Initial values: a(0)=1, a(1)=0, a(2)=1, a(3)=0, a(4)=1,
                   a(5)=1, a(6)=1, a(7)=2, a(8)=1, a(9)=3.
   The function caches already computed terms and works for
   any order of calls. */

#include <stdlib.h>
#include <stdbool.h>

static const seq_t init_vals[10] = {1,0,1,0,1,1,1,2,1,3};

static seq_t *cache = NULL;
static idx_t  cache_capacity = 0;   /* allocated size of cache   */
static idx_t  cache_last      = 0;   /* index of highest computed term */
static bool    cache_initialized = false;

/* Returns a(n) – the number of L‑pentomino tilings of a 10×n rectangle. */
seq_t a(idx_t n)
{
    /* Directly return the pre‑computed first ten terms. */
    if (n <= 9)
        return init_vals[(size_t)n];

    /* Allocate cache if it does not exist yet. */
    if (!cache) {
        idx_t init_cap = n + 1 > 10 ? n + 1 : 10;   /* at least 10 slots */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;                       /* out of memory → 0 */
        cache_capacity = init_cap;
    }

    /* Resize cache if necessary. */
    if (cache_capacity < n + 1) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                         /* reallocation failed */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Initialise first ten entries once. */
    if (!cache_initialized) {
        for (idx_t i = 0; i < 10; ++i)
            cache[i] = init_vals[i];
        cache_last = 9;
        cache_initialized = true;
    }

    /* Compute any missing values up to n. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        cache[i] = cache[i - 2] + cache[i - 5] + cache[i - 10];
    }
    cache_last = n;

    return cache[n];
}
