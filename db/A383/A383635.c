/*
 * A383635  Expansion of 1/( Product_{k=0..4} (1 - (5*k+1) * x) ).
 * https://oeis.org/A383635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 55*a(n-1) - 1085*a(n-2) + 9185*a(n-3) - 30330*a(n-4) +
 *            22176*a(n-5) (a(0) = 1, a(1) = 55, a(2) = 1940, a(3) = 56210,
 *            a(4) = 1461495)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* dynamic array of computed terms */
static size_t cache_cap = 0;         /* current allocated capacity */
static idx_t cache_last = (idx_t)-1; /* highest n that has been computed */

/* initial values for n = 0..4 */
static const seq_t base_vals[5] = {1, 55, 1940, 56210, 1461495};

seq_t a(idx_t n)
{
    /* Grow cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                 /* allocation failure – abort */
            abort();
        }
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Compute terms up to n if not already done */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i < 5) {
            cache[i] = base_vals[i];
        } else {
            /* recurrence */
            cache[i] =
                55 * cache[i - 1] -
                1085 * cache[i - 2] +
                9185 * cache[i - 3] -
                30330 * cache[i - 4] +
                22176 * cache[i - 5];
        }
    }
    cache_last = n;
    return cache[n];
}
