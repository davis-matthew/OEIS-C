/*
 * A351354  Numbers k such that the k-th centered 40-gonal numbers (A195317)
 *          is a square.
 * https://oeis.org/A351354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+18*a(n-2)-18*a(n-3)-a(n-4)+a(n-5) for n>5, a(1)=1,
 *            a(2)=3, a(3)=7, a(4)=45, a(5)=117
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑based cache, cache[0] unused */
static size_t cache_size = 0;    /* capacity of cache (number of slots) */
static size_t last_computed = 0; /* highest index that has been computed */

/* Resize the cache to hold at least n+1 entries (indices 0..n). */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_size) return;

    size_t new_size = needed * 2; /* over‑allocate a bit for future growth */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort(); /* fail fast on OOM */
    cache = new_cache;
    cache_size = new_size;
}

/* Pre‑populate the first five terms if we haven't yet. */
static void init_base_terms(void)
{
    if (last_computed >= 5) return;
    /* Ensure we have space for indices 0..5 */
    if (cache_size < 6) {
        cache_size = 6;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (!cache) abort();
    }
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)3;
    cache[3] = (seq_t)7;
    cache[4] = (seq_t)45;
    cache[5] = (seq_t)117;
    last_computed = 5;
}

/* Compute a(n) using the five‑term linear recurrence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, return 0 */
    init_base_terms();
    ensure_capacity(n);

    /* Iteratively compute missing terms up to n */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        /* recurrence: a(i) = a(i-1)+18*a(i-2)-18*a(i-3)-a(i-4)+a(i-5) */
        seq_t term =  cache[i-1];
        term += (seq_t)18 * cache[i-2];
        term -= (seq_t)18 * cache[i-3];
        term -= cache[i-4];
        term += cache[i-5];
        cache[i] = term;
    }
    last_computed = (size_t)n;
    return cache[n];
}
