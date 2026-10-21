/*
 * A271504  With a(1) = 1, a(n) is the LCM of all 0 < m < n for which a(m)
 *          divides n.
 * https://oeis.org/A271504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = LCM of all m < n such that a(m)
 *            divides n
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 27, u64 n <= 47, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static inline seq_t lcm_seq(seq_t a, seq_t b)
{
    if (a == (seq_t)0 || b == (seq_t)0) return (seq_t)0;
    return (a / gcd_seq(a, b)) * b;
}

seq_t a(idx_t n)
{
    /* Directly return the base cases for indices 1 and 2 */
    if (n <= 2) return (seq_t)1;

    /* Static cache of previously computed terms: cache[0] is unused. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Expand the cache to include indices up to n. */
    if (cache_size < n) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failed */
        cache = tmp;

        /* Initialize the base cases on the first allocation. */
        if (cache_size == 0) {
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)1;
            cache_size = 2;
        }

        /* Compute new values iteratively. */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t l = (seq_t)1;
            for (idx_t m = 1; m < i; ++m) {
                seq_t am = cache[m];
                if (am != (seq_t)0) {
                    idx_t am_i = (idx_t)am;        /* safe: am <= i <= 10000 */
                    if (i % am_i == 0) {
                        l = lcm_seq(l, (seq_t)m);
                    }
                }
            }
            cache[i] = l;
        }
        cache_size = n;
    }
    return cache[n];
}
