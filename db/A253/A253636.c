/*
 * A253636  Second partial sums of eighth powers (A001016).
 * https://oeis.org/A253636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 13, u64 n <= 131, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A253636 – Second partial sums of eighth powers
 *
 *  The sequence satisfies the recurrence
 *
 *      a(1) = 1
 *      a(2) = 258
 *      a(n) = 2*a(n-1) - a(n-2) + n^8   (n >= 3)
 *
 *  The terms for n >= 1 are returned by seq_t a(idx_t n);
 *  The implementation memoises values in an internal cache to
 *  support arbitrary ordering of calls.  All arithmetic uses the
 *  built‑in unsigned type seq_t, so the result is correct modulo
 *  2^width(seq_t) — exactly as defined by the unsigned integer
 *  semantics of C/C++.
 */

#include <stdlib.h>

/* Static cache for memoisation.  cache[0] is unused. */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;      /* allocated capacity (indices 0..cap-1) */
static size_t cache_len = 0;      /* next index to compute (cache_len <= cache_cap) */

/* Ensure the cache has at least 'needed' slots */
static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp)
        abort();                 /* out‑of‑memory */
    cache = newp;
    cache_cap = new_cap;
}

/* Compute all terms up to and including n */
static void compute_upto(idx_t n)
{
    if (n < cache_len)          /* already available */
        return;

    /* Make room for indices 0..n */
    ensure_capacity((size_t)n + 1);

    /* Initialise base values if they have not yet been set */
    if (cache_len == 0) {          /* we have no entries yet */
        cache[0] = (seq_t)0;       /* unused slot */
        cache_len = 1;
    }
    if (cache_len == 1) {          /* cache[1] not set */
        cache[1] = (seq_t)1;       /* a(1) */
        cache_len = 2;
    }
    if (cache_len == 2) {          /* cache[2] not set */
        cache[2] = (seq_t)258;     /* a(2)  (wrapped automatically if seq_t is small) */
        cache_len = 3;
    }

    /* Compute remaining terms via the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Compute i^8 */
        seq_t sn = (seq_t)i;
        seq_t pow8 = sn;
        for (int k = 1; k < 8; ++k)
            pow8 *= sn;            /* multiplication modulo 2^width(seq_t) */

        cache[i] = cache[i - 1] * 2 - cache[i - 2] + pow8;
    }

    cache_len = (size_t)n + 1;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      /* behaviour for indices below the offset is irrelevant */

    compute_upto(n);
    return cache[n];
}
