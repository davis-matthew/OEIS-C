/*
 * A303417  Number of nX4 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          4 or 5 horizontally, diagonally or antidiagonally adjacent
 *          elements, with upper left element zero.
 * https://oeis.org/A303417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) + 21*a(n-2) + 54*a(n-3) - 23*a(n-4) - 18*a(n-5)
 *            - 20*a(n-6) for n > 6 Initial values: a(1)=8, a(2)=128,
 *            a(3)=1985, a(4)=30897, a(5)=480960, a(6)=7486369. The function
 *            caches computed values for O(1) repeated accesses
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;   /* 1‑based cache (cache[0] unused)   */
static idx_t  cache_len = 0;    /* Size of allocated buffer (including index 0) */
static idx_t  max_computed = 0; /* Largest n for which cache[n] is valid      */

/* Ensure the cache can hold values up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;
    /* Allocate at least (n+1) elements; we use size_t for the size argument. */
    size_t new_len = (size_t)n + 1;
    void *p = realloc(cache, new_len * sizeof(seq_t));
    if (!p) abort();                /* Out of memory – terminate. */
    cache = (seq_t *)p;
    cache_len = new_len;
}

/* Initialise the cache with the first six terms. This is called once,
   and never reset for later calls. */
static void init_cache(void)
{
    if (max_computed >= 6) return;  /* Already initialised. */

    /* Make sure we have space for six entries (index 1–6). */
    ensure_cache(6);

    cache[0] = 0;                    /* unused entry, keeps array one‑based. */
    cache[1] = (seq_t)8;
    cache[2] = (seq_t)128;
    cache[3] = (seq_t)1985;
    cache[4] = (seq_t)30897;
    cache[5] = (seq_t)480960;
    cache[6] = (seq_t)7486369;

    max_computed = 6;
}

/* Public function that returns the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* Undefined for n<1; return 0. */

    init_cache();                   /* Ensure initial terms are defined. */

    /* Compute missing terms up to n if needed. */
    if (max_computed < n) {
        /* Grow cache if necessary. */
        ensure_cache(n);

        /* The recurrence is valid for i > 6. */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t val =
                  (seq_t)14 * cache[i-1]
                + (seq_t)21 * cache[i-2]
                + (seq_t)54 * cache[i-3]
                - (seq_t)23 * cache[i-4]
                - (seq_t)18 * cache[i-5]
                - (seq_t)20 * cache[i-6];
            cache[i] = val;
        }
        max_computed = n;
    }

    return cache[n];
}
