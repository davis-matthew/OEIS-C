/*
 * A291039  a(n) = (1/4)*A291038(n).
 * https://oeis.org/A291039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 52, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A291039
   a(n) = 4*a(n-1) - 4*a(n-2) + 2*a(n-3) - 4*a(n-4) - a(n-6)  for n >= 7
   a(0) = 1, a(1) = 3, a(2) = 8, a(3) = 21, a(4) = 54, a(5) = 136, a(6) = 337
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to store computed values for arbitrary order of calls */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;     /* number of initialized entries   */
    static idx_t cache_cap   = 0;     /* allocated capacity              */

    /* Initial values up to n = 6 */
    const seq_t init[7] = { (seq_t)1, (seq_t)3, (seq_t)8,
                            (seq_t)21, (seq_t)54, (seq_t)136, (seq_t)337 };

    /* Allocate and initialise the cache on the first call */
    if (cache == NULL) {
        cache_cap = (n >= 6) ? (n + 1) : 7;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return (seq_t)0;   /* allocation failure – unlikely in tests */

        /* Fill the known initial segment */
        for (idx_t i = 0; i < 7; ++i) cache[i] = init[i];
        cache_size = 7;
    }

    /* Grow cache if necessary */
    if (n >= cache_size) {
        if ((idx_t)(cache_cap) <= n) {
            size_t new_cap = cache_cap;
            while ((idx_t)new_cap <= n) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (cache == NULL) return (seq_t)0;   /* allocation failure – unlikely */
            cache_cap = new_cap;
        }

        /* Compute missing values up to n using the recurrence */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val = 4*cache[i-1] - 4*cache[i-2] + 2*cache[i-3]
                        - 4*cache[i-4] - cache[i-6];
            cache[i] = val;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
