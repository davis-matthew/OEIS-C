/*
 * A396065  The total number of at-most-4-furcating labeled histories that can
 *          be constructed for n labeled leaves.
 * https://oeis.org/A396065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(n-1)/2)*a(n-1) + (n*(n-1)*(n-2)/6)*a(n-2) +
 *            (n*(n-1)*(n-2)*(n-3)/24)*a(n-3) Initial values: a(1)=1, a(2)=1,
 *            a(3)=4
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑indexed array of computed terms */
static idx_t  cache_cap = 0;         /* capacity of cache (>=1) */

/* Ensure the cache can hold values up to n (inclusive).  Resize if needed. */
static void
ensure_cache(idx_t n)
{
    if (n < cache_cap) return;               /* enough space already */

    idx_t new_cap = n + 1;
    if (new_cap < cache_cap * 2) new_cap = cache_cap * 2; /* amortized growth */
    if (new_cap < 4) new_cap = 4;             /* small initial size */

#ifdef __cplusplus
    /* Allocate a new array and copy the old contents.  The new array
       is default‑constructed, so elements are initialised correctly. */
    seq_t *tmp = new seq_t[new_cap];
    if (cache) {
        for (idx_t i = 0; i < cache_cap; ++i)
            tmp[i] = cache[i];
        delete[] cache;
    }
    cache = tmp;
#else
    /* In C use realloc and explicitly zero the newly allocated part. */
    seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp) abort();                     /* out of memory */
    if (!cache) {
        for (idx_t i = 0; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
    } else {
        for (idx_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
    }
    cache = tmp;
#endif
    cache_cap = new_cap;
}

/* Return a(n) for n >= 1.  Undefined/unused for n==0. */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;                  /* no definition for 0 */

    ensure_cache(n);

    /* Initialise base cases once.  The sequence is strictly positive,
       so a zero entry indicates “not computed”. */
    if (cache[1] == (seq_t)0 && n >= 1) cache[1] = (seq_t)1;
    if (cache[2] == (seq_t)0 && n >= 2) cache[2] = (seq_t)1;
    if (cache[3] == (seq_t)0 && n >= 3) cache[3] = (seq_t)4;

    /* Compute missing entries up to n. */
    for (idx_t i = 1; i <= n; ++i) {
        if (i <= 3) continue;             /* base cases already set */
        if (cache[i] != (seq_t)0) continue; /* already known */

        /* Terms of the recurrence, computed with explicit casting. */
        seq_t t1 = ((seq_t)i * (seq_t)(i-1) / 2) * cache[i-1];
        seq_t t2 = ((seq_t)i * (seq_t)(i-1) * (seq_t)(i-2) / 6) * cache[i-2];
        seq_t t3 = ((seq_t)i * (seq_t)(i-1) * (seq_t)(i-2) *
                     (seq_t)(i-3) / 24) * cache[i-3];

        cache[i] = t1 + t2 + t3;
    }

    return cache[n];
}
