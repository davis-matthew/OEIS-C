/*
 * A375921  a(n) = LCM(1,2, ..., 2n+1)/6.
 * https://oeis.org/A375921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LCM(1,2,...,2n+1)/6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* Helper: compute lcm of two seq_t values */
static inline seq_t lcm_seq(seq_t a, seq_t b)
{
    seq_t g = gcd(a, b);
    return (a / g) * b;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 0 */

    /* static state ----------------------------------------------------- */
    static seq_t *cache = NULL;          /* cache of computed a values */
    static idx_t cache_max_n = 0;        /* maximum n cached so far */
    static seq_t cur_lcm = 1;            /* lcm of 1..cur_N */
    static idx_t cur_N = 0;              /* current N = 2*cache_max_n+1 */

    /* expand cache if needed */
    if (n > cache_max_n) {
        size_t new_size = (size_t)n;
        if (cache == NULL) {
            cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        } else {
            cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        }
    }

    /* compute missing values */
    for (idx_t idx = cache_max_n + 1; idx <= n; ++idx) {
        idx_t targetN = 2 * idx + 1;                 /* N for this n */
        for (idx_t i = cur_N + 1; i <= targetN; ++i) {
            cur_lcm = lcm_seq(cur_lcm, (seq_t)i);
        }
        cur_N = targetN;
        cache[idx - 1] = cur_lcm / (seq_t)6;         /* store a(idx) */
    }

    cache_max_n = n;
    return cache[n - 1];
}
