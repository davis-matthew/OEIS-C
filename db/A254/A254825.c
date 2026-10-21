/*
 * A254825  Number of length n 1..(6+1) arrays with every leading partial sum
 *          divisible by 2, 3 or 5.
 * https://oeis.org/A254825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑n arrays with entries 1…7 such that
 *            every * leading partial sum is divisible by 2 or 3 or 5.
 *            Implements a DP on * cumulative residues modulo 30
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static const unsigned char M_bool[30] = {
    1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,0
};
static const int M_res[22] = {
    0,2,3,4,5,6,8,9,10,12,14,15,16,18,20,21,22,24,25,26,27,28
};

seq_t a(idx_t n)
{
    /* Local static state for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t last_n = 0;          /* number of terms already computed */
    static seq_t dp_cur[30];
    static seq_t dp_next[30];

    /* Initialise DP for the first call. */
    if (cache == NULL) {
        /* dp_cur[0] corresponds to sum 0 before any elements. */
        for (int i = 0; i < 30; ++i) dp_cur[i] = 0;
        dp_cur[0] = (seq_t)1;
        last_n = 0;
    }

    if (n < 1) return 0; /* Undefined for n <= 0, but never called. */

    /* Ensure the cache can hold up to n terms. */
    if ((size_t)n > cache_size) {
        size_t new_size = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);                 /* allocation failure */
        /* Zero the newly added slots if any. */
        if (new_size > cache_size)
            memset(tmp + cache_size, 0,
                   (new_size - cache_size) * sizeof(seq_t));
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute new terms up to n. */
    while (last_n < n) {
        /* Transition from dp_cur to dp_next. */
        memset(dp_next, 0, sizeof(dp_next));

        for (int r = 0; r < 30; ++r) {
            if (dp_cur[r] == 0) continue; /* skip zero entries */
            for (int v = 1; v <= 7; ++v) {
                int res = (r + v) % 30;
                if (!M_bool[res]) continue;              /* fails the rule */
                dp_next[res] = dp_next[res] + dp_cur[r];
            }
        }

        /* Sum contributions for the new term a(last_n+1). */
        seq_t sum = 0;
        for (size_t i = 0; i < 22; ++i)
            sum = sum + dp_next[M_res[i]];

        cache[last_n] = sum;          /* store a(last_n+1) */
        /* Prepare for the next step. */
        for (int i = 0; i < 30; ++i)
            dp_cur[i] = dp_next[i];
        ++last_n;
    }

    return cache[n - 1];
}
