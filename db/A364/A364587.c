/*
 * A364587  Clique covering number, independence number, and Shannon capacity
 *          of the n-Lucas cube graph.
 * https://oeis.org/A364587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) – a(n-4) – a(n-5); Initial
 *            values: a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=6
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

static seq_t *cache = NULL;          /* 1‑based sequence values stored 0‑based */
static size_t cache_cap = 0;         /* size of allocated cache array */
static idx_t cache_max = 0;          /* largest n computed so far */

/* Ensure the cache can hold indices up to 'needed' (1‑based) */
static void ensure_capacity(idx_t needed)
{
    if ((idx_t)cache_cap >= needed)
        return;
    size_t new_cap = (size_t)needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    assert(tmp);                     /* abort on allocation failure */
    cache = tmp;
    cache_cap = new_cap;
    /* cache_max remains unchanged */
}

seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; behaviour for 0 is irrelevant */
        return (seq_t)0;

    /* At least the first five terms must exist for the recurrence */
    if (cache_max < 5) {
        /* Ensure we have space for the initial segment */
        ensure_capacity((idx_t)5);
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)2;   /* a(2) */
        cache[2] = (seq_t)3;   /* a(3) */
        cache[3] = (seq_t)4;   /* a(4) */
        cache[4] = (seq_t)6;   /* a(5) */
        cache_max = 5;
    }

    /* Grow cache to required size */
    ensure_capacity(n);

    if (n <= cache_max)
        return cache[n - 1];

    /* Extend the cache using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* Indices in cache are 0‑based: a(k) is stored at cache[k-1] */
        seq_t t1 = cache[i - 2]; /* a(i-1) */
        seq_t t2 = cache[i - 3]; /* a(i-2) */
        seq_t t3 = cache[i - 4]; /* a(i-3) */
        seq_t t4 = cache[i - 5]; /* a(i-4) */
        seq_t t5 = cache[i - 6]; /* a(i-5) */

        cache[i - 1] = t1 + t2 + t3 - t4 - t5;
    }
    cache_max = n;
    return cache[n - 1];
}
