/*
 * A261958  Start with a single square for n=0; for the odd n-th generation
 *          add a square at each expandable vertex of the squares of the
 *          (n-1)-th generation (this is the "vertex to vertex" version); for
 *          the even n-th generation use the "side to vertex" version; a(n) is
 *          the number of squares added in the n-th...
 * https://oeis.org/A261958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A261958
 *  Recurrence:  a(n) = a(n-2) + a(n-8) - a(n-10)   for n > 10
 *  Initial values:  a(0)=1, a(1)=4, a(2)=12, a(3)=16, a(4)=24,
 *                    a(5)=32, a(6)=28, a(7)=36, a(8)=32,
 *                    a(9)=44, a(10)=44
 *
 *  The function memoises previously computed values in a dynamic array,
 *  allowing out-of-order calls up to n≈10000 in well under 2 seconds.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed terms    */
static size_t cache_size = 0; /* allocated length           */

/* Ensure the cache holds at least n+1 elements */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_size)
        return;

    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure → abort */
    cache = tmp;
    /* initialise new slots to 0 (sentinel for “not yet computed”) */
    for (size_t i = cache_size; i < needed; ++i)
        cache[i] = (seq_t)0;
    cache_size = needed;
}

/* Calc of A261958 */
seq_t a(idx_t n)
{
    /* Special case: all indices ≤10 are known upfront */
    if (n <= 10) {
        static const seq_t base[11] = {
            1, 4, 12, 16, 24, 32, 28, 36, 32, 44, 44
        };
        return base[n];
    }

    ensure_cache(n);

    /* Prepare the base values in the cache if not already present */
    for (size_t i = 0; i <= (size_t)10; ++i)
        cache[i] = (seq_t)i < 11 ? (seq_t)((i <= 10) ? ((const seq_t[]){
                1, 4, 12, 16, 24, 32, 28, 36, 32, 44, 44
            }[i]) : (seq_t)0) : cache[i];

    /* Compute missing terms up to n */
    for (idx_t i = 11; i <= n; ++i) {
        if (cache[i] == (seq_t)0)
            cache[i] = cache[i-2] + cache[i-8] - cache[i-10];
    }

    return cache[n];
}
