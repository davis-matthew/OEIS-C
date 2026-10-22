/*
 * A338467  a(n+1) = prime(n) + 2*n - a(n). a(1) = 1.
 * https://oeis.org/A338467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(n+1) = prime(n) + 2*n - a(n)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"

static seq_t *cache = NULL;          /* cache[i] holds a(i+1) */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* allocated capacity */

/* grow the cache so that it can store at least up to term 'n' (n >= 1) */
static void ensure_cache(idx_t n)
{
    while (cache_len < (size_t)n) {
        /* compute a(cache_len+1) using a(cache_len) and prime(cache_len) */
        idx_t idx = cache_len;                     /* idx is the position in the
                                                       recurrence (1‑based)   */
        /* grow storage if needed */
        if (cache_cap <= cache_len) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *new_cache = (seq_t *)realloc(cache,
                                                new_cap * sizeof(seq_t));
            if (!new_cache) abort();                /* memory error */
            cache = new_cache;
            cache_cap = new_cap;
        }
        seq_t a_prev = cache[cache_len - 1];
        seq_t prime_val = (seq_t)get_prime(idx);
        seq_t val = prime_val + (seq_t)(2 * idx) - a_prev;
        cache[cache_len] = val;
        ++cache_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* defensive; offset=1 */

    /* initialise cache with a(1) if needed */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort(); /* memory error */
        cache[0] = (seq_t)1;  /* a(1) = 1 */
        cache_len = 1;
    }

    ensure_cache(n);
    return cache[n - 1];
}
