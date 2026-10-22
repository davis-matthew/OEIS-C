/*
 * A334840  a(1) = 1, a(n) = a(n-1)/gcd(a(n-1),n) if this gcd is > 1, else
 *          a(n) = 4*a(n-1).
 * https://oeis.org/A334840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n ≥ 2, a(n) = a(n‑1)/gcd(a(n‑1), n) if gcd > 1 *
 *            else a(n) = 4*a(n‑1). The sequence is memoized to allow *
 *            arbitrary order of calls
 * Range:     u8 n <= 62, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;        /* cache[0] unused, cache[1] = a(1) */
static idx_t  cache_cap = 0;       /* allocated size (indexable up to cache_cap-1) */
static idx_t  cache_len = 0;       /* highest n whose a(n) is computed */

/* Ensure cache is initialised and large enough to hold a(n) up to n. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;           /* already computed */
    /* initialise cache on first use */
    if (cache_cap == 0) {
        cache_cap = 64;                    /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[1] = (seq_t)1;               /* a(1) = 1 */
        cache_len = 1;
    }
    /* grow array if necessary */
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    /* compute missing values */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t g    = gcd(prev, (seq_t)i);
        seq_t next = (g > (seq_t)1) ? (prev / g) : (prev * (seq_t)4);
        cache[i] = next;
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but safe aborted */
    ensure_cache(n);
    return cache[n];
}
