/*
 * A283835  Number of length-n binary vectors beginning with 0, ending with 1,
 *          and avoiding 5 consecutive 0's and 5 consecutive 1's.
 * https://oeis.org/A283835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary vectors of length n that start with 0,
 *            end with 1 and contain no run of 5 consecutive 0's or 5
 *            consecutive 1's
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 69, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Sequence value type (integer or arbitrary‑precision) */
seq_t a(idx_t n)
{
    /* base case  */
    if (n == 0)
        return (seq_t)1;

    /* cache of already computed values:
       - cache == NULL  : no values computed yet
       - cache_cap      : size of allocated cache (number of entries)
       - computed_up_to : highest n whose value is known
    */
    static seq_t *cache = NULL; /* may be NULL at first */
    static idx_t cache_cap = 0;
    static idx_t computed_up_to = 0;

    /* DP state: last bit (0/1) × run length 1..4 (index 0..3) */
    static seq_t prev_state[2][4];
    static seq_t cur_state[2][4];
    static int initialized = 0;

    /* Grow cache if necessary   (C++ uses new/delete,
       plain C uses malloc/realloc) */
    if (!cache || cache_cap < n + 1) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = NULL;

#ifdef __cplusplus
        new_cache = new seq_t[new_cap];          /* type constructors run */
        if (cache) {
            for (idx_t i = 0; i < cache_cap; ++i)   /* copy existing data */
                new_cache[i] = cache[i];
            delete [] cache;                     /* destroy old array */
        }
#else
        new_cache = (seq_t *)realloc(cache,
                                   new_cap * sizeof(seq_t));
#endif
        if (!new_cache)            /* allocation failure – return 0 */
            return (seq_t)0;
        cache = new_cache;
        cache_cap = new_cap;
        if (!initialized) {          /* first allocation must set a(0)=1 */
            cache[0] = (seq_t)1;
            initialized = 1;
        }
    }

    /* Known value?      */
    if (n <= computed_up_to)
        return cache[n];

    /* First time: initialise DP for length 1   */
    if (computed_up_to == 0) {
        memset(prev_state, 0, sizeof(prev_state));
        prev_state[0][0] = (seq_t)1;      /* string "0" only */
        cache[1] = (seq_t)0;              /* cannot end with 1 at length 1 */
        computed_up_to = 1;
    }

    /* Extend DP from the last computed length up to n */
    for (idx_t len = computed_up_to + 1; len <= n; ++len) {
        memset(cur_state, 0, sizeof(cur_state));

        for (int bit = 0; bit < 2; ++bit) {
            for (int run = 0; run < 4; ++run) {
                seq_t val = prev_state[bit][run];
                if (!val)
                    continue;
                int runlen = run + 1;      /* real run length */

                /* append same bit (increase run length if <4) */
                if (runlen < 4)
                    cur_state[bit][run + 1] += val;
                /* append opposite bit (reset run length to 1) */
                cur_state[bit ^ 1][0] += val;
            }
        }

        /* sum states whose last bit is 1 */
        seq_t total = 0;
        for (int run = 0; run < 4; ++run)
            total += cur_state[1][run];
        cache[len] = total;

        /* prepare for next iteration */
        memcpy(prev_state, cur_state, sizeof(prev_state));
    }

    computed_up_to = n;
    return cache[n];
}
