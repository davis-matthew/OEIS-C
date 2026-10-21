/*
 * A279193  Least positive integer whose decimal digits divide the plane into
 *          n regions (version for people who write 2 with a curlicue).
 * https://oeis.org/A279193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + 10*a(n-2) – 10*a(n-3) for n>4; initial values:
 *            a(1)=1, a(2)=2, a(3)=8, a(4)=28
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[i] = a(i+1) for 0 ≤ i < cache_size */
static idx_t  cache_size = 0; /* number of elements currently cached */

/* ensure that cache contains a(1) … a(n) */
static void
ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;

    size_t new_size = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        abort(); /* out of memory */

    cache = new_cache;
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        switch (i) {
            case 1: cache[0] = (seq_t)1; break;
            case 2: cache[1] = (seq_t)2; break;
            case 3: cache[2] = (seq_t)8; break;
            case 4: cache[3] = (seq_t)28; break;
            default:
                cache[i-1] = cache[i-2]
                           + ((seq_t)10) * cache[i-3]
                           - ((seq_t)10) * cache[i-4];
                break;
        }
    }
    cache_size = n;
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                  /* undefined for n<1, but safe */
    ensure_cache(n);
    return cache[n-1];
}
