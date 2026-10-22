/*
 * A365974  Expansion of e.g.f. exp( Sum_{k>=0} x^(5*k+3) / (5*k+3) ).
 * https://oeis.org/A365974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Sum_{k=0..floor((n-3)/5)} a(n-5*k-3)/(n-5*k-3)!
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;          /* memoised a(i) values */
    static size_t cap = 0;               /* allocated capacity of cache   */
    static idx_t last = 0;               /* index of the largest a(i) computed */

    /* Allocate and initialise cache for a(0) on first call */
    if (cache == NULL) {
        size_t init_cap = 1;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;
        cap = init_cap;
        cache[0] = (seq_t)1;             /* a(0) = 1 */
        last = 0;
    }

    /* Grow cache if needed for index n */
    if ((idx_t)cap <= n) {
        size_t new_cap = cap;
        while ((idx_t)new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        for (size_t i = cap; i < new_cap; ++i) cache[i] = 0;
        cap = new_cap;
    }

    /* Compute any missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;             /* already set, but keep explicit */
        } else if (i < 3) {
            cache[i] = 0;                    /* a(1) = a(2) = 0 */
        } else {
            /* Use the recurrence in a multiplication‑only form:
               a(n) = Σ a(m) * ((n‑1)! / m!) with m = n‑5k‑3   */
            idx_t m = i - 3;
            seq_t ratio = (seq_t)(i - 2) * (seq_t)(i - 1);   /* (i‑2)*(i‑1) = (i‑1)!/(i‑3)! */
            seq_t sum = 0;
            while (1) {
                sum += cache[m] * ratio;
                if (m < 5) break;            /* no next term */
                /* Update ratio for next m = m‑5:
                   ratio_new = ratio * (m)*(m‑1)*(m‑2)*(m‑3)*(m‑4) */
                seq_t prod = (seq_t)m
                           * (seq_t)(m - 1)
                           * (seq_t)(m - 2)
                           * (seq_t)(m - 3)
                           * (seq_t)(m - 4);
                ratio *= prod;
                m -= 5;
            }
            cache[i] = sum;
        }
    }
    last = n;
    return cache[n];
}
