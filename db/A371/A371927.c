/*
 * A371927  Expansion of 1/(1 - x/(1 - 8*x^2)^(1/4)).
 * https://oeis.org/A371927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} 8^k * binomial((n+2*k)/4-1, k)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for previously computed values of a(n) */
    static seq_t *cache = NULL;
    static idx_t cache_sz = 0;

    /* Pre‑computed coefficients d[r] = c_{2r}
       where   c_{2r} = binom(-1/4, r) * (-8)^r
       The recurrence
           d[0] = 1,
           d[r] = d[r-1] * (4*r-3) * 2 / r
       produces these integers exactly. */
    static seq_t *d = NULL;
    static idx_t d_sz = 0;

    /* Grow the a(n) cache if necessary */
    if (cache_sz <= n) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* allocation failure */
        cache = tmp;
        for (idx_t i = cache_sz; i <= n; ++i) cache[i] = 0;
    }

    /* Determine how many d-r terms are required for the new a(n) values. */
    idx_t r_req = (n == 0) ? 0 : (n - 1) / 2;

    if (d_sz <= r_req) {
        seq_t *tmpd = (seq_t *)realloc(d, (r_req + 1) * sizeof(seq_t));
        if (tmpd == NULL) return 0;          /* allocation failure */
        d = tmpd;
        for (idx_t r = d_sz; r <= r_req; ++r) {
            if (r == 0) {
                d[0] = 1;                    /* c_0 */
            } else {
                seq_t num = d[r - 1] * (seq_t)(4 * r - 3) * 2;
                d[r] = num / (seq_t)r;       /* exact integer division */
            }
        }
        d_sz = r_req + 1;
    }

    /* Compute a(n) for all indices not yet present. */
    for (idx_t i = cache_sz; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
            continue;
        }
        seq_t sum = 0;
        idx_t rmax = (i - 1) / 2;
        for (idx_t r = 0; r <= rmax; ++r) {
            /* a(i) = Σ_{r=0}^{⌊(i-1)/2⌋} a(i-1-2r) * d[r] */
            seq_t term = cache[i - 1 - 2 * r] * d[r];
            sum += term;
        }
        cache[i] = sum;
    }
    cache_sz = n + 1;
    return cache[n];
}
