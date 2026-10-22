/*
 * A301686  Coordination sequence for node of type V1 in "krh" 2-D tiling (or
 *          net).
 * https://oeis.org/A301686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-3) - a(n-6) for n ≥ 7 a(0)=1, a(1)=5, a(2)=7,
 *            a(3)=14, a(4)=17, a(5)=19, a(6)=28 (the first six terms are
 *            derived from the given generating function)
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic array holding computed terms   */
static idx_t cache_size = 0;         /* current allocated size                    */
static idx_t computed_max = 6;       /* highest index that has been computed yet */

/* Initialise the cache with the first seven terms. */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    cache_size = 8;
    cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    if (cache == NULL)
        exit(EXIT_FAILURE);

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)5;
    cache[2] = (seq_t)7;
    cache[3] = (seq_t)14;
    cache[4] = (seq_t)17;
    cache[5] = (seq_t)19;
    cache[6] = (seq_t)28;
}

/* Grow the cache to at least size n+1.  The size is doubled until it fits. */
static void grow_cache(idx_t n)
{
    if (n < cache_size)
        return;

    size_t needed = (size_t)n + 1;
    size_t new_size = cache_size > 0 ? cache_size : 8;
    while (new_size < needed)
        new_size <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL)
        exit(EXIT_FAILURE);
    cache = tmp;
    cache_size = new_size;
}

/* Compute all terms up to n (inclusive) that are not already computed. */
static void compute_up_to(idx_t n)
{
    init_cache();
    grow_cache(n);
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* recurrence: a(i) = 2*a(i-3) - a(i-6)   (valid for i >= 7) */
        seq_t t1 = (seq_t)2 * cache[i - 3];
        cache[i] = t1 - cache[i - 6];
    }
    if (n > computed_max)
        computed_max = n;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (n <= 6) {
        /* Handle the first seven terms directly.  No growth of the cache
           is necessary for these indices, but we initialise it to keep
           the other paths working deterministically. */
        init_cache();
        return cache[n];
    }
    compute_up_to(n);
    return cache[n];
}
