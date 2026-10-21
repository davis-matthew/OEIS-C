/*
 * A291403  p-INVERT of (1,1,0,0,0,0,...), where p(S) = 1 - 2 S^2 - S^4.
 * https://oeis.org/A291403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0,a(1)=2,a(2)=4,a(3)=7,a(4)=20,a(5)=42,a(6)=92,a(7)=214,a(8)=472
 *            a(n)=2*a(n-2)+4*a(n-3)+3*a(n-4)+4*a(n-5)+6*a(n-6)+4*a(n-7)+a(n-8)
 *            for n≥9
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 55, u128 n <= 110,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t computed = 0;   /* number of elements already set in cache */

    /* Ensure we can address cache[n] */
    ensure_capacity((size_t)n + 1, &capacity, (void **)&cache, sizeof(seq_t));

    /* Base values for n < 9 */
    static const seq_t base_vals[9] = {
        0, 2, 4, 7, 20, 42, 92, 214, 472
    };

    /* Initialisation of the first few terms */
    if (computed == 0) {
        /* First time: put the known values up to the requested n
           or, if n < 8, only up to n. */
        size_t upto = (n < 8) ? (size_t)n : 8;
        for (size_t i = 0; i <= upto; ++i)
            cache[i] = base_vals[i];
        computed = upto + 1;
    }

    /* If the result was already computed, just return it */
    if ((idx_t)computed > n)
        return cache[n];

    /* If n < 9, we need to fill the remaining base values */
    if (n < 9) {
        for (size_t i = computed; i <= (size_t)n; ++i)
            cache[i] = base_vals[i];
        computed = (size_t)n + 1;
        return cache[n];
    }

    /* n ≥ 9. Ensure we have the base values for indices 0..8
       (computed is at least 9 after the above logic). */

    /* Compute the sequence up to n using the recurrence */
    for (idx_t i = (idx_t)computed; i <= n; ++i) {
        seq_t v = 2 * cache[i - 2] +
                  4 * cache[i - 3] +
                  3 * cache[i - 4] +
                  4 * cache[i - 5] +
                  6 * cache[i - 6] +
                  4 * cache[i - 7] +
                  cache[i - 8];
        cache[i] = v;
    }
    computed = (size_t)n + 1;

    return cache[n];
}
