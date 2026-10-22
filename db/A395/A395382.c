/*
 * A395382  a(n) = (2*n)! * [x^(2*n)] 1 / cos(x)^4.
 * https://oeis.org/A395382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n)! * [x^(2*n)] 1/cos(x)^4 (recurrence)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0) = 1, a(1) = 4 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build */

seq_t a(idx_t n)
{
    /* static cache for a(n) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity (index < cache_cap) */
    static size_t last_done = 0;   /* largest n already computed */

    /* static arrays for powers of 4 and 16 */
    static seq_t *pow4 = NULL;
    static size_t pow4_cap = 0;     /* capacity of pow4 (valid indices 0 .. pow4_cap-1) */
    static seq_t *pow16 = NULL;
    static size_t pow16_cap = 0;    /* capacity of pow16 */

    /* -------------------------------------------------------------
       1. Ensure cache big enough and initialise first entry.
       ------------------------------------------------------------- */
    if (cache == NULL) {
        size_t init_cap = n + 1;
        cache = (seq_t *)malloc((init_cap + 1) * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache_cap = init_cap;
        for (size_t i = 0; i <= init_cap; ++i) cache[i] = 0;
        cache[0] = 1;                 /* a(0) = 1 */
        last_done = 0;
    } else if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : n + 1;
        if (new_cap <= (size_t)n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        for (size_t i = cache_cap; i <= new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }
    /* cache[0] is already set to 1     */

    /* -------------------------------------------------------------
       2. Compute a(i) for all i from last_done+1 up to n.
       ------------------------------------------------------------- */
    for (idx_t i = last_done + 1; i <= n; ++i) {
        /* Ensure powers of 4 and 16 are available up to indices i+1 and i. */
        if (pow4_cap < (size_t)(i + 2)) {
            size_t needed = i + 2;
            size_t new_cap = pow4_cap ? pow4_cap * 2 : needed;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp4 = (seq_t *)realloc(pow4, new_cap * sizeof(seq_t));
            seq_t *tmp16 = (seq_t *)realloc(pow16, new_cap * sizeof(seq_t));
            if (!tmp4 || !tmp16) exit(EXIT_FAILURE);
            pow4 = tmp4;
            pow16 = tmp16;
            if (pow4_cap == 0) {            /* initialisation */
                pow4[0] = 1;
                pow16[0] = 1;
                pow4_cap = 1;
            }
            for (size_t j = pow4_cap; j < new_cap; ++j) {
                pow4[j] = (seq_t)pow4[j - 1] * (seq_t)4;
                pow16[j] = (seq_t)pow16[j - 1] * (seq_t)16;
            }
            pow4_cap = new_cap;
        }

        /* The recurrence involves a sum over k = 0 .. i-1. */
        seq_t pos_sum = 0;  /* Σ for even d = i-k */
        seq_t neg_sum = 0;  /* Σ for odd  d = i-k */

        seq_t b_prev = 1;   /* C(2*i, 0) */

        for (idx_t k = 0; k < i; ++k) {
            idx_t d = i - k;  /* d ≥ 1 */

            /* sumPow = 4^(d+1) + 16^d */
            seq_t sum_pow = pow4[d + 1] + pow16[d];

            /* term = a(k) * C(2*i, 2*k) * sum_pow */
            seq_t term = (seq_t)cache[k] * b_prev * sum_pow;

            if (d & 1ULL)
                neg_sum += term;
            else
                pos_sum += term;

            /* Update b_prev to C(2*i, 2*(k+1)) if needed */
            if ((k + 1) < i) {
                seq_t t1 = (seq_t)(2 * (idx_t)i - 2 * (idx_t)k);
                seq_t t2 = t1 - 1;
                seq_t t3 = (seq_t)(2 * (idx_t)(k + 1));
                seq_t t4 = t3 - 1;
                b_prev = b_prev * t1 * t2 / (t3 * t4);
            }
        }

        /* a(i) = (|neg_sum - pos_sum|) / 8   */
        seq_t diff = (neg_sum >= pos_sum) ? (neg_sum - pos_sum)
                                          : (pos_sum - neg_sum);
        cache[i] = diff / (seq_t)8;
    }

    last_done = (size_t)n;
    return cache[(size_t)n];
}
