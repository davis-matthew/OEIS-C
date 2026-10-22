/*
 * A301752  Clique covering number of the n-triangular grid graph.
 * https://oeis.org/A301752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-3) - 2*a(n-4) + 2*a(n-5) + a(n-6) -
 *            2*a(n-8) + a(n-9); Initial values a(1)=1, a(2)=3, a(3)=4,
 *            a(4)=6, a(5)=8, a(6)=10, a(7)=13, a(8)=15, a(9)=19. The sequence
 *            is computed by recurrence for n ≥ 10
 * Range:     u8 n <= 37, u16 n <= 625, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;    /* cache[0] unused, indices 1..cache_cap */
static idx_t  cache_cap = 0;   /* largest index stored in cache (0 if none) */

static const seq_t base_vals[9] = {
    (seq_t)1, (seq_t)3, (seq_t)4, (seq_t)6, (seq_t)8,
    (seq_t)10, (seq_t)13, (seq_t)15, (seq_t)19
};

/* Ensure that cache contains a(1) .. a(n).  The cache is grown
   lazily and values are computed using the recurrence. */
static void ensure_cache(idx_t n)
{
    if (n == 0 || n <= cache_cap)
        return;                          /* already available */

    size_t new_cap = (size_t)n + 1;        /* indices 0 .. n */
    if (cache == NULL) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache == NULL) abort();
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
    }

    /* Store base values for indices 1 .. 9 that are now inside the
       new cache (overwrites are harmless because the values are the same). */
    idx_t limit = new_cap < 10 ? new_cap : 10;
    for (idx_t i = 1; i < limit; ++i) {
        cache[i] = base_vals[i - 1];
    }

    /* Compute the remaining values using the recurrence. */
    idx_t start = cache_cap + 1;
    if (start < 10) start = 10;           /* recurrence only valid for n ≥ 10 */
    for (idx_t i = start; i <= n; ++i) {
        cache[i] =  2 * cache[i - 1]
                  -      cache[i - 3]
                  - 2 * cache[i - 4]
                  + 2 * cache[i - 5]
                  +      cache[i - 6]
                  - 2 * cache[i - 8]
                  +      cache[i - 9];
    }

    cache_cap = n;
}

seq_t a(idx_t n)
{
    if (n < 1) /* offset is 1; callers should use n ≥ 1 */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
