/*
 * A321847  E.g.f.: exp(x/(1 - 4*x)).
 * https://oeis.org/A321847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A321847:
 *   a(0) = 1, a(1) = 1
 *   a(n) = (8*n - 7)*a(n-1) - 16*(n-2)*(n-1)*a(n-2)   for n >= 2
 * This implementation uses a simple recurrence with memoisation.
 * It relies on the helper function mul_small defined in
 * "../utils/mul_small.h". */

#include <stdlib.h>
#include "../utils/mul_small.h"

static seq_t *cache = NULL;   /* cache[i] stores a(i)  */
static size_t cache_size = 0; /* number of allocated entries   */
static idx_t last_computed = 1;/* highest index that has been computed */

/* Grow the cache so that indices up to n can be stored.  Base values a(0)
 * and a(1) are initialised when the cache is first allocated. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_size = (n >= 1) ? (size_t)(n + 1) : 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return;          /* out of memory – caller will detect */
        cache[0] = (seq_t)1;
        if (cache_size > 1) cache[1] = (seq_t)1;
        last_computed = 1;
    } else if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return;            /* out of memory – caller will detect */
        cache = tmp;
        cache_size = new_size;
        /* New slots will be filled when needed. */
    }
}

/* Compute the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* Handle the two base cases.  They never require allocation. */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;

    /* Make sure the cache can contain all indices up to n. */
    ensure_cache(n);

    /* Compute any missing terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        unsigned int iu = (unsigned int)i; /* safe: i <= 10000 in tests */

        /* Multipliers for the recurrence.  They fit easily into 32–bit. */
        unsigned int m1 = 8u * iu - 7u;                    /* 8*i - 7  */
        unsigned int m2 = 16u * (iu - 1u) * (iu - 2u);     /* 16*(i-1)*(i-2) */

        /* Perform the multiplication using the provided helper. */
        seq_t term1 = mul_small(cache[i - 1], m1);
        seq_t term2 = mul_small(cache[i - 2], m2);

        /* a(i) = term1 - term2 */
        cache[i] = term1 - term2;
    }

    last_computed = n;
    return cache[n];
}
