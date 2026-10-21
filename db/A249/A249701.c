/*
 * A249701  Number of length n+3 0..2 arrays with every four consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining two terms.
 * https://oeis.org/A249701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) + 2*a(n-4) - 5*a(n-5) +
 *            a(n-6) - a(n-7) - 2*a(n-8) + 2*a(n-10) (valid for n ≥ 11) with
 *            initial values a(1)=39, a(2)=69, a(3)=125, a(4)=221, a(5)=377,
 *            a(6)=659, a(7)=1177, a(8)=2119, a(9)=3805, a(10)=6857
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 31, u64 n <= 67, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms for 1 ≤ n ≤ 10 */
    static const seq_t init[11] = {0,
        /*  1 */ 39,
        /*  2 */ 69,
        /*  3 */ 125,
        /*  4 */ 221,
        /*  5 */ 377,
        /*  6 */ 659,
        /*  7 */ 1177,
        /*  8 */ 2119,
        /*  9 */ 3805,
        /* 10 */ 6857
    };

    if (n <= 10) return init[n];

    /* Memoization cache: indices 1 … cache_cap-1 are valid */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated size (indices 0 … cache_cap-1) */
    static idx_t cache_last = 0;  /* highest n for which cache[n] is valid */

    /* Ensure cache has space up to n */
    if (n + 1 > cache_cap) {
        /* Allocate enough space; use casting because realloc returns void* */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!new_cache) return 0;              /* allocation failure */
        cache = new_cache;
        cache_cap = n + 1;
    }

    /* Initialise base values if this is the first time */
    if (cache_last == 0) {
        /* Copy the known initial terms into the cache */
        for (idx_t i = 1; i <= 10; ++i) cache[i] = init[i];
        cache_last = 10;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* all indices i‑k are guaranteed ≥ 1 because i ≥ 11 */
        seq_t term = 0;
        term += 3 * cache[i - 1];
        term -= 3 * cache[i - 2];
        term += 2 * cache[i - 3];
        term += 2 * cache[i - 4];
        term -= 5 * cache[i - 5];
        term +=       cache[i - 6];
        term -=       cache[i - 7];
        term -= 2 * cache[i - 8];
        term += 2 * cache[i -10];
        cache[i] = term;
    }

    cache_last = n;
    return cache[n];
}
