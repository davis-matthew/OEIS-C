/*
 * A280601  Number of nX5 0..1 arrays with no element equal to more than two
 *          of its horizontal, vertical and antidiagonal neighbors and with
 *          new values introduced in order 0 sequentially upwards.
 * https://oeis.org/A280601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 23, u64 n <= 50, u128 n <= 105,
 *            big any n
 * Helpers:   amp_alloc, amp_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence (for n>15) for OEIS A280601:
   a(n) = 5*a(n-1) - 8*a(n-2) + 4*a(n-3) + 2*a(n-6) - 2*a(n-7)
          - 3*a(n-8) + 5*a(n-9) - a(n-10) - a(n-11) - a(n-12) + a(n-13)
   Initial values (n = 1..15):
   16, 138, 293, 726, 1648, 3647, 8031, 17745, 39427, 87996,
   197061, 442244, 993767, 2234762, 5027590
*/
#include <stdlib.h>
#include <stddef.h>
#include "../utils/amp_alloc.h"
#include "../utils/amp_free.h"

extern seq_t a(idx_t n);

/* Internal cache for memoization */
static seq_t *cache = NULL;   /* array a(0)..a(max) */
static size_t cache_cap = 0;  /* number of elements allocated */

/* Initial values a(1)..a(15).  index 0 is unused. */
static const seq_t init_vals[16] = {
    0,
    16, 138, 293, 726, 1648, 3647, 8031, 17745, 39427,
    87996, 197061, 442244, 993767, 2234762, 5027590
};

/* Ensure cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;                /* indices 0..n inclusive */
    if (cache_cap >= needed) return;

    seq_t *new_cache = amp_alloc(needed);
    /* initialise all to zero */
    for (size_t i = 0; i < needed; ++i) new_cache[i] = (seq_t)0;

    /* copy existing values if any */
    for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];

    amp_free(cache);
    cache = new_cache;
    cache_cap = needed;

    /* fill initial terms 1..15 */
    size_t max_init = 15;
    if (cache_cap <= max_init) max_init = cache_cap - 1;
    for (size_t i = 1; i <= max_init; ++i) cache[i] = init_vals[i];
}

/* Main function as required */
seq_t a(idx_t n)
{
    if (n <= 15) return init_vals[(size_t)n];

    ensure_cache(n);

    /* compute values up to n (memoized) */
    for (idx_t k = 16; k <= n; ++k)
    {
        if (cache[k] != 0) continue;   /* already computed */

        seq_t val = 5 * cache[k-1]
                  - 8 * cache[k-2]
                  + 4 * cache[k-3]
                  + 2 * cache[k-6]
                  - 2 * cache[k-7]
                  - 3 * cache[k-8]
                  + 5 * cache[k-9]
                  -   cache[k-10]
                  -   cache[k-11]
                  -   cache[k-12]
                  +   cache[k-13];

        cache[k] = val;
    }
    return cache[n];
}
