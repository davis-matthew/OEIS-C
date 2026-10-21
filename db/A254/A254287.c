/*
 * A254287  Expansion of (1 - (1 - 3125*x)^(1/5)) / (625*x).
 * https://oeis.org/A254287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: (n+1)*a(n) = 625*(5*n-1)*a(n-1); a(0)=1 */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* External types defined by the build system */
extern seq_t a(idx_t n);

static seq_t *cache = NULL;   /* array holding computed terms */
static size_t cache_cap = 0;  /* number of slots allocated */
static idx_t last_computed = 0; /* highest index for which cache[i] is valid */

/* Grow the cache so that it contains values up to index n (inclusive).  */
static void ensure_cache(idx_t n)
{
    /* Allocate if this is the first call */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return;          /* out‑of‑memory – caller will fail */
        cache[0] = (seq_t)1;                /* a(0) = 1 */
        last_computed = 0;
    }
    /* Extend the memory if needed */
    else if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) return;            /* out‑of‑memory – ignore */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new values only if n is larger than we have already */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t tmp = prev;
            tmp *= (seq_t)625;
            tmp *= (seq_t)(5 * i - 1);
            cache[i] = tmp / (seq_t)(i + 1);
        }
        last_computed = n;
    }
}

seq_t a(idx_t n)
{
    if (n < 0) return (seq_t)0;      /* out of range – undefined */
    ensure_cache(n);
    if (n < (idx_t)cache_cap) return cache[n];
    /* Should never happen – safety */
    return (seq_t)0;
}
