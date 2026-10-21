/*
 * A250168  Number of length 3+1 0..n arrays with the sum of adjacent
 *          differences multiplied by some arrangement of +-1 equal to zero.
 * https://oeis.org/A250168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 29, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250168 – Number of length 3+1 0..n arrays with the sum of adjacent differences multiplied by some arrangement of +-1 equal to zero.
   Formula: a(n) = 2*a(n-1) + a(n-2) - 4*a(n-3) + a(n-4) + 2*a(n-5) - a(n-6)   (n ≥ 7)
   Initial values: a(1)=8, a(2)=37, a(3)=96, a(4)=211, a(5)=380, a(6)=639 */

#include <stdlib.h>   /* malloc, realloc, exit, EXIT_FAILURE */

static seq_t *cache     = NULL;   /* array of computed terms, indices 0..n */
static size_t cache_cap = 0;      /* current allocated capacity (size of cache) */
static idx_t   cache_max = 0;     /* highest index that has a valid value */

/* Ensure the cache has room for indices 0..need-1 (need ≥ 1). */
static void ensure_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        exit(EXIT_FAILURE);             /* memory allocation failed */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Fill the cache up to index n, using the recurrence. */
static void compute_up_to(idx_t n)
{
    if (n == 0)
        return;   /* no defined terms below offset 1 */

    /* First ensure we have initial terms up to a(6). */
    if (cache_max < 6) {
        /* Guarantee capacity for indices 0..6 */
        ensure_cache(7);
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)37;
        cache[3] = (seq_t)96;
        cache[4] = (seq_t)211;
        cache[5] = (seq_t)380;
        cache[6] = (seq_t)639;
        cache_max = 6;
    }

    if (n <= cache_max)
        return;   /* already computed */

    /* Ensure capacity for indices up to n. */
    ensure_cache((size_t)(n + 1));

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 6) {
            /* Base terms already set above; skip recurrence. */
            continue;
        }
        /* Recurrence: a(i) = 2*a(i-1) + a(i-2) - 4*a(i-3) + a(i-4) + 2*a(i-5) - a(i-6) */
        seq_t pos = (seq_t)2 * cache[i - 1];
        pos += cache[i - 2];
        pos += cache[i - 4];
        pos += (seq_t)2 * cache[i - 5];

        seq_t neg = (seq_t)4 * cache[i - 3];
        neg += cache[i - 6];

        /* Since the recurrence guarantees non‑negative result, we can subtract safely. */
        val = pos - neg;
        cache[i] = val;
    }
    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* The sequence is defined only for n ≥ 1. */
    compute_up_to(n);
    return cache[n];
}
