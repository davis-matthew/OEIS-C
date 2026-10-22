/*
 * A390221  a(n) = Sum_{k=0..floor(n/2)} binomial(k+1,4*n-8*k+1).
 * https://oeis.org/A390221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) - 6*a(n-4) + 4*a(n-6) - a(n-8) + a(n-9); Initial
 *            values: a(0)=1, a(1)=0, a(2)=2, a(3)=0, a(4)=3, a(5)=0, a(6)=4,
 *            a(7)=0, a(8)=5, a(9)=1
 * Range:     u8 n <= 20, u16 n <= 37, u32 n <= 71, u64 n <= 140,
 *            u128 n <= 275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;     /* stored computed terms */
static idx_t last_computed = (idx_t)-1; /* highest index cached */
static size_t cache_cap = 0;          /* current allocation size */

/* Ensure cache contains values up to at least n. */
static void ensure_up_to(idx_t n)
{
    /* Allocate and initialise base terms on first call. */
    if (cache == NULL) {
        size_t needed = (size_t)(n > 9 ? n + 1 : 10);
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) abort();

        cache[0] = (seq_t)1;  /* a(0) */
        cache[1] = (seq_t)0;  /* a(1) */
        cache[2] = (seq_t)2;  /* a(2) */
        cache[3] = (seq_t)0;  /* a(3) */
        cache[4] = (seq_t)3;  /* a(4) */
        cache[5] = (seq_t)0;  /* a(5) */
        cache[6] = (seq_t)4;  /* a(6) */
        cache[7] = (seq_t)0;  /* a(7) */
        cache[8] = (seq_t)5;  /* a(8) */
        cache[9] = (seq_t)1;  /* a(9) */

        cache_cap = needed;
        last_computed = 9;
    }

    /* No work if the requested index is already cached. */
    if (n <= last_computed) return;

    /* Grow the cache if necessary. */
    while ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? 2 * cache_cap : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms using the recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 9) continue;  /* base terms already set */

        seq_t t2 = (seq_t)4 * cache[i - 2];
        seq_t t4 = (seq_t)6 * cache[i - 4];
        seq_t t6 = (seq_t)4 * cache[i - 6];
        seq_t t8 = cache[i - 8];
        seq_t t9 = cache[i - 9];

        seq_t val = t2 + t6 + t9;
        val = val - t4;
        val = val - t8;

        cache[i] = val;
    }

    last_computed = n;
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return cache[n];
}
