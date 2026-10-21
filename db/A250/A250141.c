/*
 * A250141  Number of length 2+2 0..n arrays with the medians of every three
 *          consecutive terms nondecreasing.
 * https://oeis.org/A250141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5), for
 *            n>5 Initial values: a(1)=14, a(2)=67, a(3)=204, a(4)=485,
 *            a(5)=986
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* external linkage */
seq_t a(idx_t n)
{
    /* Handle undefined indices; OEIS offset starts at 1. */
    if (n == 0)
        return 0;

    /* Static cache to allow arbitrary order of calls. */
    static seq_t *cache = NULL;     /* cache[i] holds a(i) for i>=0 */
    static idx_t  cached_n = 0;     /* largest n for which cache[n] is defined */

    /* Determine required capacity. */
    idx_t req_cap = n;
    if (req_cap < 5) /* at least cover the first five terms */
        req_cap = 5;

    /* Allocate or resize the cache if necessary. */
    if (cache == NULL || cached_n < 5 || cached_n < req_cap) {
        /* Ensure capacity: grow by factor 2 or at least to req_cap. */
        idx_t new_cap = (cached_n < 5) ? 5 : cached_n * 2;
        if (new_cap < req_cap)
            new_cap = req_cap;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (new_cap + 1));
        if (new_cache == NULL)
            return 0; /* allocation failed – fallback */
        cache = new_cache;
    }

    /* Initialize first five terms if they are not already set. */
    if (cached_n < 5) {
        cache[0] = 0;          /* unused but convenient */
        cache[1] = 14;
        cache[2] = 67;
        cache[3] = 204;
        cache[4] = 485;
        cache[5] = 986;
        cached_n = 5;
    }

    /* Compute terms up to requested n using the recurrence. */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        cache[i] = 5 * cache[i-1]
                 - 10 * cache[i-2]
                 + 10 * cache[i-3]
                 - 5  * cache[i-4]
                 + cache[i-5];
    }
    cached_n = n;

    return cache[n];
}
