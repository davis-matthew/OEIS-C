/*
 * A259767  Number of (n+2)X(3+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000001 or 00000101.
 * https://oeis.org/A259767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 37, u64 n <= 84, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-3) + 3*a(n-6) + 4*a(n-7) - 2*a(n-9)
   + 2*a(n-11) - 8*a(n-13) + 6*a(n-14)   for n > 17
   Initial values:
   a(1)=104, a(2)=167, a(3)=256, a(4)=385, a(5)=612,
   a(6)=1108, a(7)=1708, a(8)=2700, a(9)=4660, a(10)=7378,
   a(11)=11350, a(12)=19232, a(13)=31280, a(14)=48032,
   a(15)=80218, a(16)=131336, a(17)=202926 */
#include <stddef.h>
#include <stdlib.h>

/* static cache of computed terms */
static seq_t *cache = NULL;      /* array of values, index matches n */
static size_t  cache_cap = 0;    /* allocated capacity (elements) */
static idx_t   cache_max = 0;    /* maximum n currently computed */

/* initialize cache with known initial values */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    /* indices 0..17; 0 unused */
    const seq_t init_vals[18] = {
        0ULL, 104ULL, 167ULL, 256ULL, 385ULL, 612ULL, 1108ULL, 1708ULL,
        2700ULL, 4660ULL, 7378ULL, 11350ULL, 19232ULL, 31280ULL, 48032ULL,
        80218ULL, 131336ULL, 202926ULL
    };

    cache_cap = 18;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) return;  /* allocation failed; caller will get 0 */

    for (size_t i = 0; i < cache_cap; ++i)
        cache[i] = init_vals[i];

    cache_max = 17;   /* terms up to a(17) are now known */
}

/* ensure cache has capacity for at least n+1 elements */
static void ensure_capacity(idx_t n)
{
    if (n < cache_cap)
        return;

    size_t new_cap = cache_cap;
    while (new_cap <= (size_t)n)
        new_cap <<= 1; /* grow exponentially */

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        return; /* if realloc fails, keep old cache; caller will get 0 */

    cache = new_cache;
    cache_cap = new_cap;
}

/* public function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n=0, but return 0 gracefully */

    init_cache();
    if (!cache) return 0; /* allocation failure */

    if (n > cache_max) {
        ensure_capacity(n);
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            /* compute using recurrence */
            cache[(size_t)i] = 3 * cache[(size_t)(i - 3)] +
                               3 * cache[(size_t)(i - 6)] +
                               4 * cache[(size_t)(i - 7)] -
                               2 * cache[(size_t)(i - 9)] +
                               2 * cache[(size_t)(i - 11)] -
                               8 * cache[(size_t)(i - 13)] +
                               6 * cache[(size_t)(i - 14)];
        }
        cache_max = n;
    }

    return cache[(size_t)n];
}
