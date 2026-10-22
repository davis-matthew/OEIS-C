/*
 * A313836  Coordination sequence Gal.3.23.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n)=a(n-1)+a(n-7)-a(n-8) for n>8,
   with a(0)=1, a(1)=5, a(2)=10, a(3)=16, a(4)=20,
   a(5)=26, a(6)=31, a(7)=36, a(8)=41 */

#include <stdlib.h>   /* abort */
#include <stddef.h>   /* size_t */

#include "../utils/seq_alloc.h"   /* seq_alloc(seq_t* cache, size_t cap)   */
#include "../utils/seq_realloc.h" /* seq_realloc(seq_t* cache, size_t old_cap, size_t new_cap) */

/* ---------- Internal cache ----------------------------------- */
static seq_t *cache = NULL;   /* array of computed terms */
static size_t cap = 0;        /* allocated capacity (in elements) */
static idx_t max_n = 0;       /* highest index currently computed */

/* ---------- Helper: grow and fill the cache -------------------- */
static void ensure_cache(idx_t n)
{
    /* Initialise the cache with the first 9 terms */
    if (cache == NULL) {
        cap = 9;                       /* indices 0..8 inclusive */
        cache = seq_alloc(cap);
        if (cache == NULL) abort();

        cache[0] = 1;
        cache[1] = 5;
        cache[2] = 10;
        cache[3] = 16;
        cache[4] = 20;
        cache[5] = 26;
        cache[6] = 31;
        cache[7] = 36;
        cache[8] = 41;
        max_n = 8;
    }

    /* Nothing to do if the requested term is already known */
    if (n <= max_n)
        return;

    /* Grow the capacity if necessary */
    size_t needed = (size_t)n + 1;
    if (cap < needed) {
        size_t new_cap = cap * 2;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = seq_realloc(cache, cap, new_cap);
        if (tmp == NULL) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* Compute the missing terms using the recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* a(i) = a(i-1) + a(i-7) - a(i-8) for i>8 */
        seq_t v1 = cache[(size_t)(i - 1)];
        seq_t v2 = cache[(size_t)(i - 7)];
        seq_t v3 = cache[(size_t)(i - 8)];
        cache[(size_t)i] = v1 + v2 - v3;
    }

    max_n = n;
}

/* ---------- Public sequence function --------------------------- */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
