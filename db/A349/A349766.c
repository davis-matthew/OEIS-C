/*
 * A349766  Numbers of the form 2*t^2 - 4 when t > 1 is a term in A001541.
 * https://oeis.org/A349766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 35*a(n-1) - 35*a(n-2) + a(n-3) for n ≥ 4, with a(1)=14,
 *            a(2)=574, a(3)=19598
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;       /* array of terms, 1‑indexed */
static idx_t cache_size = 0;     /* number of terms stored (0 means none) */

static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;
    idx_t new_size = n;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed – terminate program (no I/O permitted) */
        abort();
    }
    cache = new_cache;
    /* initialise new slots */
    for (idx_t i = cache_size + 1; i <= new_size; ++i) {
        if (i == 1)          cache[i] = (seq_t)14;
        else if (i == 2)     cache[i] = (seq_t)574;
        else if (i == 3)     cache[i] = (seq_t)19598;
        else {
            seq_t v1 = cache[i-1];
            seq_t v2 = cache[i-2];
            seq_t v3 = cache[i-3];
            cache[i] = v1 * (seq_t)35 - v2 * (seq_t)35 + v3;
        }
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for offset 1, but safe */
    ensure_cache(n);
    return cache[n];
}
