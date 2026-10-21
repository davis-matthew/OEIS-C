/*
 * A249804  a(n+1) is the next smallest nontrivial cube beginning with a(n),
 *          initial term is 4.
 * https://oeis.org/A249804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   floor_cube_root, pow10, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description: a(1) = 4.
   For n≥1, a(n+1) is the smallest *non‑trivial* cube > a(n) whose
   decimal representation begins with that of a(n).  "Non‑trivial" means
   that the cube’s integer root is not divisible by 10 (i.e., the cube
   does not end in zeros). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/pow10.h"                /* seq_t pow10(idx_t e); */
#include "../utils/floor_cube_root.h"      /* seq_t floor_cube_root(seq_t x); */
#include "../utils/seq_alloc_new_array.h"  /* seq_t *seq_alloc_new_array(size_t); */
#include "../utils/seq_realloc.h"          /* seq_t *seq_realloc(seq_t*, size_t, size_t); */

/* Cached sequence values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap   = 0;

/* Ensure the cache can store at least `need` terms */
static void ensure_cache(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_cache;
    if (!cache)
        new_cache = seq_alloc_new_array(new_cap);
    else
        new_cache = seq_realloc(cache, cache_cap, new_cap);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the next cube that starts with `prev` and whose root is not
   a multiple of 10 (non‑trivial). */
static seq_t next_cube_with_prefix(seq_t prev)
{
    for (unsigned k = 0; ; ++k) {
        seq_t pow10k = pow10((idx_t)k);
        seq_t lower  = prev * pow10k;
        seq_t upper  = (prev + (seq_t)1) * pow10k - (seq_t)1;

        /* Determine the cube‑root bounds for this prefix interval */
        seq_t r_low  = floor_cube_root(lower - (seq_t)1) + (seq_t)1;
        seq_t r_high = floor_cube_root(upper);

        while (r_low <= r_high) {
            if (r_low % (seq_t)10 == 0) {   /* skip trivial cubes */
                ++r_low;
                continue;
            }
            seq_t cube = r_low * r_low * r_low;
            if (cube > prev) return cube;
            ++r_low;
        }
        /* No cube found in this range – increase the number of trailing
           zeros and try again. */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for n=0 */

    ensure_cache(n);

    while (cache_size < n) {
        if (cache_size == 0) {
            cache[0] = (seq_t)4;            /* a(1) = 4 */
        } else {
            seq_t prev = cache[cache_size - 1];
            cache[cache_size] = next_cube_with_prefix(prev);
        }
        ++cache_size;
    }
    return cache[n - 1];
}
