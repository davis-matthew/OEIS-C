/*
 * A392431  a(n) = Sum_{k=0..2*n} binomial(3*k,2*n-k).
 * https://oeis.org/A392431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A392431
 * Recurrence (n >= 4):
 *   a(n) = 7*a(n-1) - a(n-2) + 3*a(n-3) - a(n-4)
 * Initial values:
 *   a(0) = 1,  a(1) = 4,  a(2) = 26,  a(3) = 181
 *
 * The function caches previously computed terms so that it works
 * correctly for arbitrary order of calls.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cached values.  The cache is expanded only as needed. */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;

/* Initialise or grow the cache up to index n.  The function may
 * use standard malloc/realloc/free because seq_t is a plain C type
 * or a C++ class that supports mallocable storage. */
static void ensure_cache(idx_t n)
{
    /* Nothing to do if we already have all terms up to n. */
    if (cache_len > n) return;

    /* Allocate (or enlarge) the array. */
    idx_t new_len = n + 1;                     /* indices 0 .. n inclusive   */
    seq_t *tmp = (seq_t *)realloc(cache,
                                  new_len * sizeof(seq_t));

    if (!tmp) abort();                         /* allocation failure */
    cache = tmp;

    /* Compute any missing terms. */
    for (idx_t i = cache_len; i <= n; ++i) {
        switch (i) {
            case 0:  cache[i] = (seq_t)1;      break;
            case 1:  cache[i] = (seq_t)4;      break;
            case 2:  cache[i] = (seq_t)26;     break;
            case 3:  cache[i] = (seq_t)181;    break;
            default:
                /* a(i) = 7*a(i-1) - a(i-2) + 3*a(i-3) - a(i-4) */
                seq_t val = 7 * cache[i-1];
                val -= cache[i-2];
                val += 3 * cache[i-3];
                val -= cache[i-4];
                cache[i] = val;
                break;
        }
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
