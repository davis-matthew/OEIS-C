/*
 * A263999  Number of (n+1) X (2+1) arrays of permutations of 0..n*3+2 with
 *          each element having index change (+-,+-) 0,0 1,2 or 1,0.
 * https://oeis.org/A263999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 37*a(n-2) - 44*a(n-3) - 256*a(n-4) + 64*a(n-5)
 *            + 256*a(n-6) (n ≥ 7) Initial values: a(1) = 18 a(2) = 99 a(3) =
 *            925 a(4) = 7304 a(5) = 62101 a(6) = 516117
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* cache[0] unused; cache[i] = a(i)  */
static size_t cache_len = 0;     /* number of cached entries (cache_len-1 = highest index cached) */

static const seq_t base_vals[7] = {0, 18, 99, 925, 7304, 62101, 516117};

/* Ensure that indices 0..n are cached, allocating if necessary */
static void ensure_cache(idx_t n)
{
    if (n < 1) return;                          /* below offset not needed */

    size_t needed = (size_t)n + 1;              /* include index n */
    if (cache_len >= needed) return;            /* already cached */

    /* Reallocate cache to needed size */
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    cache = tmp;

    /* Fill new entries */
    for (size_t i = cache_len; i < needed; ++i)
    {
        if (i <= 6)
        {
            cache[i] = base_vals[i];
        }
        else
        {
            /* Compute recurrence:
               a(i) = 5*a(i-1) + 37*a(i-2) - 44*a(i-3) - 256*a(i-4)
                      + 64*a(i-5) + 256*a(i-6) */
            seq_t pos = 5 * cache[i-1] + 37 * cache[i-2]
                        + 64 * cache[i-5] + 256 * cache[i-6];
            seq_t neg = 44 * cache[i-3] + 256 * cache[i-4];
            cache[i] = pos - neg;
        }
    }
    cache_len = needed;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;       /* below offset */
    ensure_cache(n);
    return cache[n];
}
