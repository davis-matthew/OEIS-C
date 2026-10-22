/*
 * A314127  Coordination sequence Gal.6.326.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence: a(0..12) = {1,5,11,16,21,27,31,37,42,47,53,58,63}
 * a(n) = a(n-1) + a(n-11) - a(n-12)  for n > 12
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for previously computed values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* ensure cache has at least size n+1 */
static void _ensure_cache(idx_t n)
{
    if (cache_cap == 0) {
        cache_cap = n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* initialise base terms (safe to do every time) */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)5;
    cache[2] = (seq_t)11;
    cache[3] = (seq_t)16;
    cache[4] = (seq_t)21;
    cache[5] = (seq_t)27;
    cache[6] = (seq_t)31;
    cache[7] = (seq_t)37;
    cache[8] = (seq_t)42;
    cache[9] = (seq_t)47;
    cache[10] = (seq_t)53;
    cache[11] = (seq_t)58;
    cache[12] = (seq_t)63;
}

seq_t a(idx_t n)
{
    if (n <= 12) {
        switch ((idx_t)n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)5;
            case 2: return (seq_t)11;
            case 3: return (seq_t)16;
            case 4: return (seq_t)21;
            case 5: return (seq_t)27;
            case 6: return (seq_t)31;
            case 7: return (seq_t)37;
            case 8: return (seq_t)42;
            case 9: return (seq_t)47;
            case 10: return (seq_t)53;
            case 11: return (seq_t)58;
            case 12: return (seq_t)63;
            default: return (seq_t)0;   /* unreachable */
        }
    }

    _ensure_cache(n);

    for (idx_t i = 13; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-11] - cache[i-12];
    }

    return cache[n];
}
