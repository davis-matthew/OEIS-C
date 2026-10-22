/*
 * A396496  Number of ways to write n as a sum n = 0 + 1 + 2 +- 3 +- 4 ... +-
 *          m with nonnegative partial sums and minimal m >= 0 allowing at
 *          least one way.
 * https://oeis.org/A396496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Count ways to write n as 0+1+2±3±4…±m with all partial sums
 *            non‑negative and minimal m. Uses DP on the partial sums
 * Range:     u8 n <= 346, u16 n <= 2206, u32 n <= 9543, u64 n <= 9543,
 *            u128 n <= 9543, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
/* cast for C++   */
#define CAST(ptr, type) reinterpret_cast<type>(ptr)
#else
/* cast for C     */
#define CAST(ptr, type) ((type)(ptr))
#endif

seq_t a(idx_t n)
{
    /* Cache of already computed terms (optional).  */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;

    /* Return cached value if we already have it. */
    if (n < cache_size) {
        /* All sequence values are non‑zero.  */
        if (cache[n] != 0)
            return cache[n];
    }

    /* --------- dynamic programming from scratch --------- */
    /* Start with terms 0,1,2 already fixed: partial sum 3. */
    size_t cur_cap = 3;                /* current maximum sum (=T_m). */
    seq_t *dp_cur = CAST(calloc(cur_cap + 1, sizeof(seq_t)), seq_t *);
    if (!dp_cur) return 0;             /* allocation failure → 0. */
    dp_cur[3] = (seq_t)1;              /* one way to reach 3. */

    /* Iterate adding terms i = 3,4,5,… until n is reachable. */
    for (idx_t i = 3; ; ++i) {
        size_t new_cap = cur_cap + (size_t)i;   /* new maximum sum */
        seq_t *dp_next = CAST(calloc(new_cap + 1, sizeof(seq_t)), seq_t *);
        if (!dp_next) {
            free(dp_cur);
            return 0;
        }

        /* transition */
        for (idx_t s = 0; s <= cur_cap; ++s) {
            seq_t val = dp_cur[s];
            if (val == (seq_t)0) continue;      /* nothing to extend */

            /* plus i */
            dp_next[s + i] += val;

            /* minus i, only if the resulting sum is non‑negative */
            if (s >= i)
                dp_next[s - i] += val;
        }

        free(dp_cur);
        dp_cur = dp_next;
        cur_cap = new_cap;

        /* Check if we have already reached n.  */
        if (n <= cur_cap && dp_cur[n] != (seq_t)0) {
            seq_t result = dp_cur[n];

            /* Expand cache if needed and store the result. */
            if (n >= cache_size) {
                size_t new_size = (size_t)n + 1;
                seq_t *new_cache = CAST(realloc(cache, new_size * sizeof(seq_t)), seq_t *);
                if (!new_cache) {
                    free(dp_cur);
                    return result; /* return result even if we cannot cache */
                }
                /* initialize new slots to zero (default‑constructed). */
                for (size_t k = cache_size; k < new_size; ++k)
                    new_cache[k] = (seq_t)0;
                cache = new_cache;
                cache_size = new_size;
            }
            cache[n] = result;
            free(dp_cur);
            return result;
        }
    }
}
