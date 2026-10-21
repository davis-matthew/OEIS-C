/*
 * A250891  Number of (n+1) X (1+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 23*a(n-2) + 28*a(n-3) - 12*a(n-4), n>6 with
 *            initial values a(1)=37, a(2)=125, a(3)=413, a(4)=1333,
 *            a(5)=4213, a(6)=13111
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Static cache holding computed values: index 0 is unused. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Grow the cache to hold indices up to `need-1`. */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap) return;
    /* allocate a new buffer */
    seq_t *new_cache = (seq_t *)malloc(need * sizeof(seq_t));
    if (!new_cache) return;                /* allocation failed – caller will
                                              get zero or garbage later */
    /* copy old data */
    if (cache) memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
    /* zero new slots */
    memset(new_cache + cache_cap, 0, (need - cache_cap) * sizeof(seq_t));
    free(cache);
    cache = new_cache;
    cache_cap = need;
}

/* Compute terms up to n, filling cache. */
static void compute_upto(idx_t n)
{
    if (n < 1) return;
    size_t up = (size_t)n + 1;     /* need indices 1..n inclusive */
    ensure_cache(up);

    for (size_t i = 1; i <= (size_t)n; ++i) {
        if (cache[i] != 0) continue;        /* already known */

        switch (i) {
            case 1: cache[i] = (seq_t)37;   break;
            case 2: cache[i] = (seq_t)125;  break;
            case 3: cache[i] = (seq_t)413;  break;
            case 4: cache[i] = (seq_t)1333; break;
            case 5: cache[i] = (seq_t)4213; break;
            case 6: cache[i] = (seq_t)13111; break;
            default:
                /* recurrence uses the four previous terms which are
                   guaranteed to have been computed earlier in this loop */
                cache[i] = 8 * cache[i - 1]
                          - 23 * cache[i - 2]
                          + 28 * cache[i - 3]
                          - 12 * cache[i - 4];
                break;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* out of range: return 0 */
    compute_upto(n);
    return cache[(size_t)n];
}
