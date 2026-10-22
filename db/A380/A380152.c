/*
 * A380152  Decimal expansion of 864/275.
 * https://oeis.org/A380152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the nth decimal digit of 864/275 * (int part first, then
 *            fractional digits from the repeating decimal)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 1‑based cache of digits: cache[1] = a(1), etc. */
    static seq_t *cache      = NULL;   /* digit array */
    static idx_t cache_cap   = 0;       /* capacity excluding index 0 (invalid) */
    static idx_t cache_len   = 0;       /* highest index computed so far (>=1 after init) */
    static uint64_t remainder = 0;      /* remainder after computing cache_len */

    if (n == 0)          /* OEIS offset starts at 1; return 0 for safety */
        return (seq_t)0;

    /* On first call, initialise cache with room for a few digits. */
    if (cache_len == 0) {
        cache_cap = 32;                       /* initial capacity (1‑based) */
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));  /* +1 because of 1‑based index */

        /* integer part of 864/275 is 3 */
        cache[1] = (seq_t)(864 / 275);        /* 3 */
        cache_len = 1;
        remainder = 864 % 275;                /* 39, the remainder before fractional digits */
    }

    /* Ensure the cache can hold up to index n. */
    if (n > cache_cap) {
        /* Grow until new_cap > n.  +1 keeps the 1‑based indexing safe. */
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failure: return 0 as fallback */
        cache      = new_cache;
        cache_cap  = new_cap;
    }

    /* Compute digits until we reach desired index. */
    while (cache_len < n) {
        /* Next digit of the fractional part: floor( remainder*10 / 275 ) */
        uint64_t prod  = remainder * 10ULL;
        uint64_t digit = prod / 275ULL;
        remainder = prod % 275ULL;
        cache_len++;
        cache[cache_len] = (seq_t)digit;
    }

    return cache[n];
}
