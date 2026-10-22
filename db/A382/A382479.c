/*
 * A382479  Number of palindromic binary strings of length n having no 6-runs
 *          of 1's.
 * https://oeis.org/A382479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = even_f[n/2] (n even) a(n) = global_f[n/2] + even_f[n/2]
 *            (n odd) where global_f[m] counts binary strings of length m with
 *            no 6 consecutive 1’s, even_f[m] counts such strings whose
 *            trailing run of 1’s is at most 2
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 64, u64 n <= 128,
 *            u128 n <= 258, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *global_f = NULL;           /* 2^m up to runs>5 forbidden       */
    static seq_t *even_f   = NULL;           /* same, but suffix ones ≤2          */
    static size_t array_sz = 0;              /* size of arrays (≥ m+1)           */
    static seq_t dp_cur[6] = {0};            /* current DP for runs 0..5          */
    static int initialized = 0;

    if (!initialized) {
        array_sz = 1;
        global_f = (seq_t *)malloc(array_sz * sizeof(seq_t));
        even_f   = (seq_t *)malloc(array_sz * sizeof(seq_t));
        if (!global_f || !even_f) return (seq_t)0;   /* allocation failed */
        global_f[0] = even_f[0] = (seq_t)1;
        dp_cur[0] = (seq_t)1;
        initialized = 1;
    }

    /* number of bits needed in the first half (floor(n/2)) */
    idx_t m = n >> 1;

    /* extend DP arrays up to required length */
    if (array_sz <= (size_t)m) {
        size_t new_sz = (size_t)m + 1;
        seq_t *new_global = (seq_t *)realloc(global_f, new_sz * sizeof(seq_t));
        seq_t *new_even   = (seq_t *)realloc(even_f,   new_sz * sizeof(seq_t));
        if (!new_global || !new_even) return (seq_t)0;   /* realloc failed */
        global_f = new_global;
        even_f   = new_even;

        for (size_t len = array_sz; len <= (size_t)m; ++len) {
            seq_t next[6] = {0};

            for (int l = 0; l <= 5; ++l) {
                seq_t val = dp_cur[l];
                if (!val) continue;
                /* choose bit 0 */
                next[0] += val;
                /* choose bit 1, if run < 5 */
                if (l < 5)
                    next[l + 1] += val;
            }

            /* store results */
            seq_t sum_global = next[0] + next[1] + next[2] +
                               next[3] + next[4] + next[5];
            seq_t sum_even   = next[0] + next[1] + next[2];

            global_f[len] = sum_global;
            even_f[len]   = sum_even;

            /* prepare for next iteration */
            for (int l = 0; l < 6; ++l)
                dp_cur[l] = next[l];
        }
        array_sz = new_sz;
    }

    /* compute result */
    seq_t ans = even_f[m];
    if (n & 1ULL)          /* odd n */
        ans = global_f[m] + ans;
    return ans;
}
