/*
 * A384677  Expansion of (1-x-2*x^2) / (1-2*x-4*x^2+2*x^3).
 * https://oeis.org/A384677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) - 2*a(n-3); Initial values: a(0)=1,
 *            a(1)=1, a(2)=4
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* realloc, abort */

static seq_t *cache   = NULL;   /* cached values */
static size_t cache_cap = 0;    /* allocated slots */
static size_t computed_up_to = 0; /* highest index already computed */

/* make sure capacity is at least up_to+1 */
static void ensure_cache_size(size_t up_to)
{
    if (cache_cap > up_to)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap <= up_to)
        new_cap = up_to + 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0)
        abort();                 /* memory allocation failed */

    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    /* Handle the first three terms directly. */
    if (idx == 0)
        return (seq_t)1;
    if (idx == 1)
        return (seq_t)1;
    if (idx == 2)
        return (seq_t)4;

    ensure_cache_size(idx);

    /* Initialise the base values if not yet done. */
    if (computed_up_to < 3) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        computed_up_to = 2;
    }

    if (idx <= computed_up_to)
        return cache[idx];

    for (size_t i = computed_up_to + 1; i <= idx; ++i) {
        cache[i] =
            2 * cache[i-1] + 4 * cache[i-2] - 2 * cache[i-3];
    }
    computed_up_to = idx;
    return cache[idx];
}
