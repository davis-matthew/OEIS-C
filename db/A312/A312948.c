/*
 * A312948  Coordination sequence Gal.5.66.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312948
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

/* OEIS A312948
   Coordination sequence Gal.5.66.2
   Recurrence for n > 10:
      a(n) =  2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4)
            + 2*a(n-5) - 2*a(n-6) + 2*a(n-7) - 2*a(n-8)
            + 2*a(n-9) - a(n-10)
   Initial values:
      a(0)=1, a(1)=4, a(2)=9, a(3)=13, a(4)=18, a(5)=23,
      a(6)=28, a(7)=33, a(8)=37, a(9)=42, a(10)=46
*/

#include <stdlib.h>

/* Static cache for memoization */
static seq_t *cache = NULL;          /* array of seq_t values */
static size_t cache_cap = 0;         /* allocated capacity */
static size_t cache_len = 0;         /* number of valid entries (>= cache_len == next free index) */

static void compute_to(idx_t n)
{
    /* The first 11 terms are known explicitly. */
    const seq_t init[11] = { 1, 4, 9, 13, 18, 23, 28, 33, 37, 42, 46 };
    size_t need = (size_t)n + 1;          /* number of entries that must exist */

    /* Ensure enough storage.  We grow by a factor of 2 for amortized linear behaviour. */
    if (need > cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq_t *newc = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!newc) abort();               /* fatal, nothing else we can do */

        if (cache) {
            for (size_t i = 0; i < cache_len; ++i) newc[i] = cache[i];
            free(cache);
        }
        cache = newc;
        cache_cap = new_cap;
    }

    /* Initialise the missing initial values up to index 10 (if needed). */
    if (need > cache_len) {
        size_t init_end = (need > 10) ? 11 : need;   /* last index to initialise */
        for (size_t i = cache_len; i < init_end; ++i)
            cache[i] = init[i];
        cache_len = init_end;

        /* Compute further terms using the recurrence. */
        for (size_t i = cache_len; i < need; ++i) {
            /* Use a signed intermediate type to avoid unsigned underflow.
               long long is large enough for all practical test ranges. */
            long long t =  2LL * (long long)cache[i-1]
                         - 2LL * (long long)cache[i-2]
                         + 2LL * (long long)cache[i-3]
                         - 2LL * (long long)cache[i-4]
                         + 2LL * (long long)cache[i-5]
                         - 2LL * (long long)cache[i-6]
                         + 2LL * (long long)cache[i-7]
                         - 2LL * (long long)cache[i-8]
                         + 2LL * (long long)cache[i-9]
                         - (long long)cache[i-10];
            cache[i] = (seq_t)t;
        }
        cache_len = need;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    compute_to(n);
    return cache[(size_t)n];
}
