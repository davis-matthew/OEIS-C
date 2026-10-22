/*
 * A317759  Number of nX2 0..1 arrays with every element unequal to 0, 1 or 2
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A317759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-3) - a(n-4) for n>6 with initial
 *            values: a(1)=2, a(2)=6, a(3)=10, a(4)=20, a(5)=42, a(6)=89
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 58, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* array a[0..cache_len] */
static size_t cache_cap = 0;          /* current allocated capacity (indexable up to cache_cap-1) */
static size_t cache_len = 0;          /* largest index computed so far */

/* Ensure the cache is at least of size `need+1` and
   contains the initial six values. */
static void
ensure_cache(idx_t need)
{
    if (need <= cache_len) return;                /* already available */

    /* Grow the capacity if needed. */
    if (cache_cap <= (size_t)need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap <= (size_t)need) new_cap = need + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize base cases if not already set. */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;      /* unused */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)10;
        cache[4] = (seq_t)20;
        cache[5] = (seq_t)42;
        cache[6] = (seq_t)89;
        cache_len = 6;
        /* If need < 6, we already have enough. */
    }

    /* Compute further values up to `need` using recurrence. */
    for (idx_t i = (idx_t)cache_len + 1; i <= need; i++) {
        if (i <= 6) {
            /* Already set by initial block. Leave as is. */
            continue;
        } else {
            cache[i] =
                ( (seq_t)2 * cache[i - 1] )
                + cache[i - 2]
                - cache[i - 3]
                - cache[i - 4];
        }
    }
    cache_len = (size_t)need;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not defined, but safe */
    ensure_cache(n);
    return cache[n];
}
