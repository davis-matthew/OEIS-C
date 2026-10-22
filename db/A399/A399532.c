/*
 * A399532  G.f. A(x) satisfies: A(x) = (1 - x^2 + x * A(2*x)) / (1 - x^3).
 * https://oeis.org/A399532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A399532
   Recurrence:
       a(0) = a(1) = a(2) = 1
       a(n) = 2^(n-1) * a(n-1) + a(n-3)   for n ≥ 3
   Offset: 0
*/

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return the constant initial terms for n < 3. */
    if (n < 3)
        return (seq_t)1;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = 2;           /* up to index 2 is defined */
    static seq_t pow_of_two = 4;             /* 2^2, value needed for i = 3 */

    /* Ensure enough space for index n. */
    if (cache == NULL) {
        size_t init_cap = (n + 1 > 3) ? (n + 1) : 3;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;               /* allocation failure */
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        cache_cap = init_cap;
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 3) new_cap = 3;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;           /* allocation failure */
        for (idx_t i = 0; i <= max_computed; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed. */
    if (n <= max_computed)
        return cache[n];

    /* Compute missing terms, updating pow_of_two each step. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = pow_of_two * cache[i - 1] + cache[i - 3];
        pow_of_two <<= 1;                    /* next power of two */
    }

    max_computed = n;
    return cache[n];
}
