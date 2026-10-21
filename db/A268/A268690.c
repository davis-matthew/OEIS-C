/*
 * A268690  A268642 with a(n+1) = 5*n if ceiling(a(n)/2) is already in the
 *          sequence.
 * https://oeis.org/A268690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n≥1, let v=a(n) and w=(v+1)/2. If w already occurs
 *            among a(1)…a(n) then a(n+1)=5*v, otherwise a(n+1)=w
 * Range:     u8 n <= 12, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t cache_len = 0;      /* number of stored terms */

/* Ensure that the cache can hold at least 'need' elements. */
static void _ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
    while (new_cap < need)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                 /* allocation failure */

    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term of the sequence (n≥1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;         /* undefined for offset 1, safety guard */

    /* Initialise once with a(1)=1. */
    if (cache_len == 0) {
        _ensure_cache(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Extend the cache up to index n. */
    while (cache_len < n) {
        seq_t last = cache[cache_len - 1];
        seq_t w    = (last + (seq_t)1) / (seq_t)2;   /* ceil(last/2) */

        /* Check whether w has appeared before. */
        bool found = false;
        for (size_t i = 0; i < cache_len; ++i) {
            if (cache[i] == w) {
                found = true;
                break;
            }
        }

        seq_t next = found ? (seq_t)5 * last : w;

        _ensure_cache(cache_len + 1);
        cache[cache_len++] = next;
    }

    return cache[(size_t)(n - 1)];
}
