/*
 * A330405  a(1) = 0; thereafter a(n) = (a(n-1)^2+1) mod n.
 * https://oeis.org/A330405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 0; for n > 1: a(n) = (a(n-1)^2 + 1) mod n
 * Range:     u8 n <= 20, u16 n <= 679, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 9415 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* undefined for n < 1 */

    static seq_t *cache   = NULL;
    static idx_t cache_size = 0;          /* largest index that has been computed */

    /* If we already have the value cached, return it immediately. */
    if (n <= cache_size) return cache[n];

    /* allocate a new cache large enough for indices 0 … n (index 0 unused) */
    idx_t new_cap = n + 1;
    seq_t *new_cache = mem_new(seq_t, (size_t)new_cap);
    if (!new_cache) {
        /* allocation failed – fallback to recomputing without a cache */
        seq_t res = 0;
        if (n == 1) return res;
        seq_t prev = 0;
        for (idx_t i = 2; i <= n; ++i) {
            res = prev * prev + 1;
            res %= (seq_t)i;
            prev = res;
        }
        return res;
    }

    /* copy the old values into the new cache (avoid dereferencing a NULL pointer) */
    if (cache != NULL && cache_size > 0) {
        for (idx_t i = 0; i <= cache_size; ++i)
            new_cache[i] = cache[i];
    }

    /* compute any missing terms */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1) {
            new_cache[1] = 0;              /* initial value */
        } else {
            seq_t prev = new_cache[i - 1];
            seq_t val  = prev * prev + 1;
            val %= (seq_t)i;
            new_cache[i] = val;
        }
    }

    /* free old cache and keep new one */
    mem_free(cache);
    cache = new_cache;
    cache_size = n;

    return cache[n];
}
