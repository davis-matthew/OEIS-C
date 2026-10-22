/*
 * A303717  Number of nX6 0..1 arrays with every element unequal to 0, 1 or 5
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 56, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+a(n-2)+3*a(n-3)+a(n-4)-a(n-5)-a(n-6), n > 9
   Initial values: a(1)=13,a(2)=13,a(3)=24,a(4)=76,a(5)=169,
                    a(6)=343,a(7)=741,a(8)=1618,a(9)=3451 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;           /* array of computed terms  */
static size_t cache_cap = 0;          /* allocated capacity (in seq_t elements) */
static idx_t cache_max = 0;           /* largest n for which a(n) is computed */

/* Ensure cache can hold terms up to n inclusive; initialise first 9 terms. */
static void ensure_cache(idx_t n)
{
    /* first call or enlarge the allocation */
    if (cache == NULL) {
        cache_cap = (size_t)(n + 1) > 10 ? (size_t)(n + 1) : 10;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);

        static const seq_t init[9] = {
            (seq_t)13, (seq_t)13, (seq_t)24, (seq_t)76,
            (seq_t)169, (seq_t)343, (seq_t)741,
            (seq_t)1618, (seq_t)3451
        };
        for (idx_t i = 1; i <= 9; ++i) cache[i] = init[i-1];
        cache_max = 9;
    } else if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n + 1)) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Compute a(n) using the recurrence and memoisation. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, return 0 safely */

    ensure_cache(n);

    if (n <= cache_max)           /* already computed */
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i > 9) {
            seq_t sum = cache[i-1] + cache[i-2] + (seq_t)3 * cache[i-3] +
                        cache[i-4] - cache[i-5] - cache[i-6];
            cache[i] = sum;
        } /* else indices 1..9 already initialised */
    }
    cache_max = n;
    return cache[n];
}
