/*
 * A374096  Number of edge covers of fan graph F_{1,n}.
 * https://oeis.org/A374096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-3) - 2*a(n-4), with a(1)=1, a(2)=4,
 *            a(3)=16, a(4)=59
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* 1‑based cache, dynamic; the cache may grow to hold any index requested. */
static seq_t *cache = NULL;        /* array of seq_t */
static idx_t cache_cap = 0;        /* number of slots currently allocated */
static idx_t last_computed = 0;    /* largest n for which cache[n] is valid */

/* enlarge the cache to at least (n+1) slots (indices 0..n). */
static void ensure_capacity(idx_t n)
{
    if (n < cache_cap)
        return;                                   /* already enough */
    idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap <= n)
        new_cap = n + 1;                            /* guarantee n+1 slots */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                                   /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* initialise the cache with the first four terms */
static void init_cache(void)
{
    if (cache)
        return;                                   /* already initialised */
    ensure_capacity(4);
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)4;
    cache[3] = (seq_t)16;
    cache[4] = (seq_t)59;
    last_computed = 4;
}

/* sequence entry: may be called in any order */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                                   /* below offset */

    init_cache();                                   /* ensure base terms */

    /* make sure we can address cache[n] */
    ensure_capacity(n);

    if (n > last_computed) {
        // compute new values up to n
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t t1 = (seq_t)4 * cache[i - 1];
            seq_t t2 = (seq_t)5 * cache[i - 3];
            seq_t t3 = (seq_t)2 * cache[i - 4];
            cache[i] = t1 - t2 - t3;                // guaranteed non‑negative
        }
        last_computed = n;
    }
    return cache[n];
}
