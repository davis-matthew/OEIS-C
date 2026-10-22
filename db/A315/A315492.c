/*
 * A315492  Coordination sequence Gal.6.625.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12 Initial values (n =
 *            0 .. 12) taken from the OEIS entry
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* -- pre‑computed initial block ------------------------------------ */
    static const seq_t init[13] = {
        (seq_t)1,  (seq_t)6,  (seq_t)11, (seq_t)16,
        (seq_t)22, (seq_t)28, (seq_t)32, (seq_t)38,
        (seq_t)44, (seq_t)49, (seq_t)54, (seq_t)60,
        (seq_t)66
    };

    /* -- memoised cache ----------------------------------------------- */
    static seq_t   *cache   = NULL;   /* array of previously computed terms   */
    static size_t  cache_size = 0;    /* number of terms currently stored    */
    static size_t  cache_cap  = 0;    /* allocated capacity (elements)       */

    /* initialise cache on first call */
    if (cache == NULL)
    {
        cache_cap   = 13;
        cache_size  = 13;
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
#endif
        for (size_t i = 0; i < 13; ++i)
            cache[i] = init[i];
    }

    /* extend capacity if we are asked for a term beyond the current cache */
    if ((size_t)n >= cache_size)
    {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap)
            new_cap = new_cap * 2 + 2;        /* growth factor is generous */

#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
        for (size_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
        cache = new_cache;
#else
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {        /* allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache = new_cache;
#endif
        cache_cap = new_cap;

        /* compute missing terms using the recurrence            */
        for (size_t i = cache_size; i <= (size_t)n; ++i)
        {
            /* a(i) = a(i-1) + a(i-11) - a(i-12)  (valid for i >= 13) */
            cache[i] = cache[i-1] + cache[i-11] - cache[i-12];
        }
        cache_size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
