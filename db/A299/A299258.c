/*
 * A299258  Coordination sequence for 3D uniform tiling formed by stacking
 *          parallel layers of the 4.6.12 2D tiling (cf. A072154).
 * https://oeis.org/A299258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 165, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A299258: Coordination sequence for 3D uniform tiling formed by stacking
 * parallel layers of the 4.6.12 2D tiling.
 * Recurrence for n > 7:
 *   a(n) = 2*a(n‑1) – a(n‑2) + a(n‑5) – 2*a(n‑6) + a(n‑7)
 * Initial values (n = 0 … 7):
 *   a(0)=1, a(1)=5, a(2)=13, a(3)=25, a(4)=41,
 *   a(5)=62, a(6)=89, a(7)=121
 */

#include <stdlib.h>

static const seq_t init_vals[8] = {
    (seq_t)1, (seq_t)5, (seq_t)13, (seq_t)25,
    (seq_t)41, (seq_t)62, (seq_t)89, (seq_t)121
};

static seq_t *seq_cache   = NULL;   /* dynamic array of computed values   */
static size_t cache_cap   = 0;       /* current allocated capacity (entries) */
static idx_t  cache_max   = (idx_t)-1;/* highest index that has been computed */

seq_t a(idx_t n)
{
    /* Initialise cache on first call (must be done before any lookup). */
    if (seq_cache == NULL) {
        cache_cap = 8;
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (seq_cache == NULL) return (seq_t)0;  /* allocation failure */

        for (size_t i = 0; i < 8; ++i)
            seq_cache[i] = init_vals[i];

        cache_max = 7;           /* we now know a(0)…a(7) are valid */
    }

    /* If the requested index is already cached, return it immediately. */
    if (n <= cache_max)
        return seq_cache[n];

    /* Grow the cache so it can hold indices 0 … n. */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)(n + 1) > new_cap) {
            if (new_cap > SIZE_MAX / 2) {
                new_cap = n + 1;
                break;
            }
            new_cap <<= 1;  /* double the capacity */
        }
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all terms from cache_max+1 up to n (inclusive). */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* For indices 0 … 7 the initial constants are already stored. */
        if (i >= 8) {
            seq_t val = 2 * seq_cache[i - 1];
            val -= seq_cache[i - 2];
            val += seq_cache[i - 5];
            val -= 2 * seq_cache[i - 6];
            val += seq_cache[i - 7];
            seq_cache[i] = val;
        } else {
            seq_cache[i] = init_vals[i];
        }
    }
    cache_max = n;
    return seq_cache[n];
}
