/*
 * A397274  G.f. A(x) satisfies (1-3*x)*A(x) + A(x^3) = x.
 * https://oeis.org/A397274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n >= 2 a(n) = 3*a(n-1) - a(n/3) if n mod 3 == 0
 *            a(n) = 3*a(n-1) otherwise
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* External function required by the test harness */
seq_t a(idx_t n)
{
    /* cache[0] is unused, cache[1] = a(1) */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;                    /* highest index stored */

    if (n == 0) return 0;                           /* undefined but harmless */

    /* Ensure the cache is large enough */
    if (n > max_cached) {
        idx_t new_size = (max_cached == 0) ? 1 : max_cached << 1;
        while (new_size < n + 1) new_size <<= 1;    /* make room for n+1 entries */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);         /* allocation failure */
        cache = new_cache;

        /* initialize new slots to 0 (not strictly required but safe) */
        for (idx_t i = max_cached + 1; i < new_size; ++i) cache[i] = 0;

        if (max_cached < 1) {                       /* set a(1) = 1 */
            cache[1] = 1;
            max_cached = 1;
        }

        /* Compute values up to n */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            if (i % 3 == 0)
                cache[i] = 3 * cache[i - 1] - cache[i / 3];
            else
                cache[i] = 3 * cache[i - 1];
        }
        max_cached = n;
    }

    return cache[n];
}
