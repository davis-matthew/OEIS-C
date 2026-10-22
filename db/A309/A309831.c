/*
 * A309831  Number of even parts appearing among the smallest parts of the
 *          partitions of n into 5 parts.
 * https://oeis.org/A309831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 139, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A309831 – Number of even parts appearing among the smallest parts 
    of the partitions of n into 5 parts.
    Recurrence (valid for n > 19):
        a(n) = a(n-1)+a(n-2)-2*a(n-5)+a(n-8)+a(n-9)
               -a(n-11)-a(n-12)+2*a(n-15)-a(n-18)-a(n-19)+a(n-20)
    Initial values (n = 0…20):
        0,0,0,0,0,0,0,0,0,0,1,1,2,3,5,6,9,11,15,18,24
*/

#include <stdlib.h>

static const seq_t init_vals[21] = {
    0ULL,0ULL,0ULL,0ULL,0ULL,
    0ULL,0ULL,0ULL,0ULL,0ULL,
    1ULL,1ULL,2ULL,3ULL,5ULL,
    6ULL,9ULL,11ULL,15ULL,18ULL,24ULL
};

static seq_t *cache      = NULL;  /* cached values */
static size_t  capacity  = 0;     /* allocated size of cache */
static size_t  max_comp  = 0;     /* highest index computed */

/* Ensure that cache has room for indices 0..n and that values up to n are computed. */
static void
ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (capacity < needed) {
        /* grow the cache */
        size_t new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;          /* allocation failure – caller will see zeros */

        cache = tmp;
        /* initialise the newly allocated part to zero */
        for (size_t i = capacity; i < new_cap; ++i) cache[i] = 0;

        /* copy the initial values over the relevant part   */
        size_t init_len = new_cap < 21 ? new_cap : 21;
        for (size_t i = 0; i < init_len; ++i) cache[i] = init_vals[i];

        /* keep track of the highest computed index   */
        if (new_cap <= 21)
            max_comp = new_cap - 1;
        else
            max_comp = (max_comp < 20) ? 20 : max_comp;

        capacity = new_cap;
    }

    /* Compute any missing terms using the recurrence   */
    if ((size_t)n > max_comp) {
        size_t start = (max_comp + 1 < 21) ? 21 : max_comp + 1;
        for (size_t i = start; i <= (size_t)n; ++i) {
            cache[i] = cache[i-1] + cache[i-2]
                       - 2*cache[i-5] + cache[i-8] + cache[i-9]
                       - cache[i-11] - cache[i-12]
                       + 2*cache[i-15] - cache[i-18] - cache[i-19] + cache[i-20];
        }
        max_comp = (size_t)n;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
