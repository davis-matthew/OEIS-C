/*
 * A301139  Coordination sequence T1 for Zeolite Code NPO.
 * https://oeis.org/A301139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 167, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coordination sequence T1 for Zeolite Code NPO (OEIS A301139)
 *
 * Recurrence (valid for n ≥ 14):
 *   a(n) = 3*a(n-4) - 3*a(n-8) + a(n-12)
 * Initial values for 0 ≤ n ≤ 13 are taken from the OEIS entry.
 *
 * The function uses a static cache so terms can be computed in any order.
 */

#include <stdlib.h>    /* malloc, realloc, NULL */

static const seq_t init_terms[14] = {
    1ULL, 4ULL, 10ULL, 20ULL, 34ULL, 58ULL, 82ULL,
    108ULL, 144ULL, 186ULL, 222ULL, 268ULL, 330ULL, 386ULL
};

static seq_t *cache = NULL;          /* pointer to cached values   */
static idx_t  cache_capacity = 0;    /* allocated size            */
static idx_t  cache_size     = 0;    /* number of computed terms   */

/* Ensure the cache can hold at least 'min_capacity' entries.
   If the cache is empty, it is created and the first 14 terms are
   filled in.  The cache is never shrunk. */
static void
ensure_cache(idx_t min_capacity)
{
    if (cache_capacity >= min_capacity)
        return;

    size_t new_cap = (size_t)min_capacity;
    seq_t *new_ptr = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_capacity = min_capacity;

    /* Initialise the first 14 terms if we haven't done so already. */
    if (cache_size < 14) {
        for (idx_t i = cache_size; i < 14; ++i)
            cache[i] = init_terms[i];
        cache_size = 14;
    }
}

/* Public sequence routine.  It may be called with any n ≥ 0 in any
   order, and it will compute the requested term efficiently. */
seq_t a(idx_t n)
{
    /* Return the known initial values directly. */
    if (n < 14)
        return init_terms[n];

    /* Initialise or grow the cache as needed. */
    if (cache == NULL)
        ensure_cache(14);

    if (n >= cache_capacity)
        ensure_cache(n + 1);

    /* Return cached value if already computed. */
    if (n < cache_size)
        return cache[n];

    /* Compute new terms up to n using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* All indices below 14 are guaranteed to have been initialised. */
        seq_t v = 3 * cache[i - 4]
                - 3 * cache[i - 8]
                +  cache[i - 12];
        cache[i] = v;
    }

    cache_size = n + 1;
    return cache[n];
}
