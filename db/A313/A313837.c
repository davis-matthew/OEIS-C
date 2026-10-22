/*
 * A313837  Coordination sequence Gal.6.618.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313837
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

/* A313837 coordination sequence
   Formula – linear recurrence for n > 10:
      a(n) = a(n-1) + a(n-3) – a(n-4) – a(n-6) + a(n-7) + a(n-9) – a(n-10)
   Initial values:
      a(0)=1, a(1)=5, a(2)=10, a(3)=16, a(4)=20,
      a(5)=26, a(6)=31, a(7)=36, a(8)=42, a(9)=46, a(10)=52
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store already computed terms   */
    static seq_t *cache = NULL;
    static size_t cache_cap   = 0;   /* allocated capacity                 */
    static size_t cache_size  = 0;   /* number of terms stored (0..size-1) */

    /* initial values for indices 0..10 (constant time) */
    static const seq_t init[11] = {
        (seq_t)1,  (seq_t)5,  (seq_t)10, (seq_t)16, (seq_t)20,
        (seq_t)26, (seq_t)31, (seq_t)36, (seq_t)42, (seq_t)46, (seq_t)52
    };

    /* return early for the first 11 terms */
    if (n <= 10)
        return init[(size_t)n];

    /* enlarge cache if necessary (doubling strategy) */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;                     /* new_cap *= 2 */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                    /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* copy the initial block into the cache if not yet present */
    if (cache_size <= 10) {
        for (size_t i = 0; i <= 10; ++i)
            cache[i] = init[i];
        cache_size = 11;
    }

    /* compute missing terms up to index n using the recurrence   */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        cache[i] = cache[i-1] + cache[i-3]
                   - cache[i-4] - cache[i-6]
                   + cache[i-7] + cache[i-9]
                   - cache[i-10];
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
