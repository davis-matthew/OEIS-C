/*
 * A297297  Number of nX6 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 2 neighboring 1s.
 * https://oeis.org/A297297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) -8*a(n-3) -2*a(n-4) +4*a(n-5) +4*a(n-6) +a(n-7)
 *            -a(n-8) (n > 8) Initial values: a(1) = 1 a(2) = 6 a(3) = 17 a(4)
 *            = 50 a(5) = 157 a(6) = 495 a(7) = 1574 a(8) = 5031
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* dynamic cache of computed values */
static idx_t  cache_cap = 0;         /* allocated capacity (elements)      */
static idx_t  cache_len = 0;         /* actual length used (highest index+1) */
static idx_t  computed_up_to = 8;    /* highest n computed so far           */
static bool   init_done = false;     /* flag to set initial values once      */

static void ensure_cache(idx_t n)
{
    /* Allocate or grow cache to hold indices up to n */
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap == 0 ? 16 : cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (!new_cache) exit(1);    /* fatal error if allocation fails */
        if (cache) {
            memcpy(new_cache, cache, sizeof(seq_t) * cache_len);
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }
    if (cache_len <= n) cache_len = n + 1;
}

static void init_values(void)
{
    /* Set the first eight terms (1‑based indexing) */
    cache[1] = 1;
    cache[2] = 6;
    cache[3] = 17;
    cache[4] = 50;
    cache[5] = 157;
    cache[6] = 495;
    cache[7] = 1574;
    cache[8] = 5031;
    init_done = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1 */

    ensure_cache(n);

    if (!init_done) init_values();

    if (n <= computed_up_to)         /* already computed */
        return cache[n];

    /* Compute values up to n using the recurrence */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        cache[i] = 4 * cache[i - 1]
                 - 8 * cache[i - 3]
                 - 2 * cache[i - 4]
                 + 4 * cache[i - 5]
                 + 4 * cache[i - 6]
                 + cache[i - 7]
                 - cache[i - 8];
    }
    computed_up_to = n;
    return cache[n];
}
