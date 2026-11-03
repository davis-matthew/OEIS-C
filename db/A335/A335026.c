/*
 * A335026  a(n) = (n + 1)^2*a(n - 2) + a(n - 1), starting 0, 9, ....
 * https://oeis.org/A335026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + 1)^2 * a(n - 2) + a(n - 1) with a(1) = 0, a(2) = 9
 *            (offset 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *a_cache = NULL;   /* memoized values, indexed from 0 */
static idx_t  cache_cap = 0;    /* allocated capacity of a_cache   */
static idx_t  cache_len = 0;    /* highest index already computed */

seq_t a(idx_t n) {
    /* trivial cases */
    if (n == 0)             return (seq_t)0;
    if (n == 1)             return (seq_t)0;
    if (n == 2)             return (seq_t)9;

    /* initialise cache on first complex request */
    if (a_cache == NULL) {
        cache_cap = 64;
        a_cache = mem_new(seq_t, cache_cap);
        if (!a_cache) return (seq_t)0;          /* allocation failure */
        a_cache[0] = (seq_t)0;                 /* unused */
        a_cache[1] = (seq_t)0;                 /* a(1) */
        a_cache[2] = (seq_t)9;                 /* a(2) */
        cache_len = 2;
    }

    /* enlarge cache if needed */
    if (n > cache_len) {
        if (n + 1 > cache_cap) {
            idx_t new_cap = cache_cap;
            while (n + 1 > new_cap) new_cap <<= 1;
            seq_t *tmp = mem_resize(seq_t, a_cache, new_cap);
            if (!tmp) return a_cache[cache_len]; /* keep last safe value */
            a_cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t m = (seq_t)(i + 1);
            a_cache[i] = m * m * a_cache[i - 2] + a_cache[i - 1];
        }
        cache_len = n;
    }

    return a_cache[n];
}
