/*
 * A334293  First quadrisection of Padovan sequence.
 * https://oeis.org/A334293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) + a(n-3) with a(0)=1, a(1)=0, a(2)=2
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Grow the cache array so that it is large enough for indices up to n */
static void ensure_cache_size(idx_t n, seq_t **ptr, size_t *cap)
{
    idx_t needed = n + 1;
    if (*cap >= (size_t)needed) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap * 2);
    while (new_cap < (size_t)needed) new_cap *= 2;
    /* cast for C++ compatibility */
    seq_t *tmp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    if (tmp == NULL) { abort(); }
    *ptr   = tmp;
    *cap   = new_cap;
}

seq_t a(idx_t n)
{
    /* Make sure the cache can hold index n */
    ensure_cache_size(n, &cache, &cache_cap);

    /* Initialize the first three values on every call (idempotent) */
    cache[0] = (seq_t)1;
    if (n >= 1) cache[1] = (seq_t)0;
    if (n >= 2) cache[2] = (seq_t)2;

    /* Return the requested term if it is one of the base cases */
    if (n == 0) return cache[0];
    if (n == 1) return cache[1];
    if (n == 2) return cache[2];

    /* Compute all terms up to n using the recurrence */
    for (idx_t i = 3; i <= n; ++i) {
        cache[i] = 2 * cache[i-1] + 3 * cache[i-2] + cache[i-3];
    }
    return cache[n];
}
