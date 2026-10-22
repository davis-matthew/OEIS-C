/*
 * A315557  Coordination sequence Gal.6.663.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   alloc_seq_array, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS formula: 
   a(n) = a(n-7) + a(n-13) - a(n-20) for n > 20
   a(0)=1, a(1)=6, a(2)=11, a(3)=18, a(4)=22, a(5)=27, a(6)=31, 
   a(7)=41, a(8)=47, a(9)=48, a(10)=54, a(11)=59, a(12)=68, a(13)=72, 
   a(14)=78, a(15)=85, a(16)=88, a(17)=94, a(18)=97, a(19)=105, a(20)=113
*/

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/grow_seq_array.h"

static const seq_t base_values[21] = {
    1ULL, 6ULL, 11ULL, 18ULL, 22ULL, 27ULL,
    31ULL, 41ULL, 47ULL, 48ULL, 54ULL, 59ULL,
    68ULL, 72ULL, 78ULL, 85ULL, 88ULL, 94ULL,
    97ULL, 105ULL, 113ULL
};

/* Cache of computed values: 0 .. computed_max */
static seq_t *cache      = NULL;   /* dynamic array */
static size_t cache_cap  = 0;      /* allocated capacity */
static idx_t  computed_max = -1;   /* highest index whose value is known */

/* Return the coordination sequence value a(n). */
seq_t a(idx_t n)
{
    /* Handle the initial segment directly. */
    if (n < 21) {
        return base_values[(size_t)n];
    }

    /* Initialise the cache on first use. */
    if (cache == NULL) {
        cache_cap = 21;                      /* store indices 0..20 */
        cache     = alloc_seq_array(cache_cap);
        for (size_t i = 0; i < 21; ++i) {
            cache[i] = base_values[i];
        }
        computed_max = 20;                   /* last pre‑filled index */
    }

    /* Grow the cache if necessary (use safe reallocation for non‑POD). */
    if ((size_t)n + 1 > cache_cap) {
        grow_seq_array(&cache, &cache_cap, (size_t)n + 1);
    }

    /* Compute missing values up to n. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        cache[i] = cache[i - 7] + cache[i - 13] - cache[i - 20];
    }
    computed_max = n;

    return cache[n];
}
