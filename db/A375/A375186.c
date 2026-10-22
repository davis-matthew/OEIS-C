/*
 * A375186  Number of subsets of {1,2,...,n} such that no two elements differ
 *          by 1, 2, 4, or 5.
 * https://oeis.org/A375186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 34, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+a(n-3)-a(n-4)+a(n-6)  for n>=6
   Initial values: a(0)=1, a(1)=2, a(2)=3, a(3)=4, a(4)=6, a(5)=8 */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static const seq_t init_vals[6] = {1, 2, 3, 4, 6, 8};

static seq_t *cache      = NULL;  /* allocated array for a(0)..a(max)   */
static size_t cache_cap  = 0;      /* array capacity                       */
static size_t computed   = 0;      /* number of terms that have been
                                      actually computed and stored       */

/* enlarge the cache so that indices up to n (inclusive) can be stored */
static void ensure_cache(idx_t n)
{
    /* n is known to be small (≤ 32768 in test harness), so cast safely */
    size_t need = (size_t)n + 1U;
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 64U;
    while (new_cap < need)
        new_cap <<= 1U;
    seq_t *tmp;
    if (cache == NULL)
        tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
    else
        tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache      = tmp;
    cache_cap  = new_cap;
}

/* compute the n-th term of the sequence, any order of calls is allowed */
seq_t a(idx_t n)
{
    if (n < 6U)
        return init_vals[(size_t)n];

    ensure_cache(n);

    /* initialise the first six terms the first time */
    if (computed < 6U) {
        for (size_t i = computed; i < 6U; ++i)
            cache[i] = init_vals[i];
        computed = 6U;
    }

    /* fill in missing terms up to n */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        /* recurrence: a(i) = a(i-1)+a(i-3)-a(i-4)+a(i-6) */
        cache[i] = cache[i - 1U] + cache[i - 3U] - cache[i - 4U] + cache[i - 6U];
    }
    computed = (size_t)n + 1U;   /* now a(0)..a(n) are computed */

    return cache[(size_t)n];
}
