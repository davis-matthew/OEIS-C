/*
 * A311826  Coordination sequence Gal.5.91.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A311826 – Coordination sequence Gal.5.91.5
   Recurrence (for n > 18):
      a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5)
             + a(n-6) - a(n-7) + a(n-8) - a(n-9) + a(n-10) - a(n-11)
   Initial values (n = 0 … 18):
      1, 4, 8, 13, 17, 21, 25, 31, 34, 42,
      45, 48, 52, 58, 61, 70, 72, 74, 78
*/

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;        /* dynamic array of computed terms        */
static size_t cache_size = 0;      /* number of terms currently stored       */
static size_t cache_cap  = 0;      /* allocated capacity of cache            */

static const seq_t init_vals[19] = {
    1ULL,4ULL,8ULL,13ULL,17ULL,21ULL,25ULL,31ULL,34ULL,42ULL,
    45ULL,48ULL,52ULL,58ULL,61ULL,70ULL,72ULL,74ULL,78ULL
};

/* Ensure cache is large enough to store at least indices 0 … n */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap <= (size_t)n + 1)
        new_cap *= 2;

    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
#else
    new_cache = malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();      /* memory allocation failure */
#endif

    /* Copy old data */
    for (size_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

    /* Deallocate old storage */
    if (cache) {
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }

    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    /* Initialise first 19 terms once */
    if (cache_size == 0) {
        ensure_cache_capacity(18);
        for (size_t i = 0; i < 19; ++i)
            cache[i] = init_vals[i];
        cache_size = 19;
    }

    if (n < cache_size)
        return cache[(size_t)n];

    ensure_cache_capacity(n);

    for (idx_t i = cache_size; i <= n; ++i) {
        /* compute by recurrence for i > 18 */
        seq_t val = cache[(size_t)(i-1)]  /* + */
                  - cache[(size_t)(i-2)]  /* - */
                  + cache[(size_t)(i-3)]  /* + */
                  - cache[(size_t)(i-4)]  /* - */
                  + cache[(size_t)(i-5)]  /* + */
                  + cache[(size_t)(i-6)]  /* + */
                  - cache[(size_t)(i-7)]  /* - */
                  + cache[(size_t)(i-8)]  /* + */
                  - cache[(size_t)(i-9)]  /* - */
                  + cache[(size_t)(i-10)] /* + */
                  - cache[(size_t)(i-11)];/* - */
        cache[(size_t)i] = val;
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
