/*
 * A328431  Number of inversion sequences of length n avoiding the consecutive
 *          patterns 010, 021, 120, and 210.
 * https://oeis.org/A328431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: DP recurrence for inversion sequences avoiding patterns 010, 021,
   120, and 210.  The recurrence keeps track of the last two entries of an
   inversion sequence, and for each possible new entry v extends all
   permissible prefixes so as to avoid the forbidden consecutive triples. */

#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#include <vector>
#include <utility>
#endif

seq_t a(idx_t n)
{
    /* offset 0, 1 */
    if (n == 0 || n == 1)
        return 1;

    size_t max_n = (size_t)n + 1;

#if defined(__cplusplus)
    std::vector<seq_t> eq_vec(max_n, 0), dist_vec(max_n, 0),
                       eq_n_vec(max_n, 0), dist_n_vec(max_n, 0);
    seq_t *eq   = eq_vec.data();
    seq_t *dist = dist_vec.data();
    seq_t *eq_n = eq_n_vec.data();
    seq_t *dist_n = dist_n_vec.data();
#else
    seq_t *eq   = (seq_t*)calloc(max_n, sizeof(seq_t));
    seq_t *dist = (seq_t*)calloc(max_n, sizeof(seq_t));
    seq_t *eq_n = (seq_t*)calloc(max_n, sizeof(seq_t));
    seq_t *dist_n = (seq_t*)calloc(max_n, sizeof(seq_t));
#endif

    /* length‑2 base cases: 00 and 01 */
    eq[0]   = 1;      /* [0,0]   */
    dist[1] = 1;      /* [0,1]   */

    size_t cur_len = 2;

    while (cur_len < (size_t)n) {
        size_t next_len = cur_len + 1;

#if defined(__cplusplus)
        std::fill(eq_n,   eq_n   + next_len, seq_t{0});
        std::fill(dist_n, dist_n + next_len, seq_t{0});
#else
        memset(eq_n,   0, sizeof(seq_t) * next_len);
        memset(dist_n, 0, sizeof(seq_t) * next_len);
#endif

        /* totals of current arrays */
        seq_t total_eq = 0, total_dist = 0;
        for (size_t v = 0; v < cur_len; ++v) {
            total_eq   += eq[v];
            total_dist += dist[v];
        }

        /* prefix sums of dist */
#if defined(__cplusplus)
        std::vector<seq_t> prefix_vec(cur_len);
        seq_t *prefix = prefix_vec.data();
#else
        seq_t *prefix = (seq_t*)malloc(cur_len * sizeof(seq_t));
#endif
        seq_t acc = 0;
        for (size_t v = 0; v < cur_len; ++v) {
            acc += dist[v];
            prefix[v] = acc;
        }

        /* transition to length next_len */
        for (size_t new_v = 0; new_v < next_len; ++new_v) {
            if (new_v < cur_len) {
                eq_n[new_v]   += eq[new_v];
                dist_n[new_v] += total_eq - eq[new_v];
            } else {
                dist_n[new_v] += total_eq;
            }

            if (new_v < cur_len) {
                eq_n[new_v]   += dist[new_v];
                if (new_v > 0)
                    dist_n[new_v] += prefix[new_v - 1];
            } else {
                dist_n[new_v] += prefix[cur_len - 1];
            }
        }

#if !defined(__cplusplus)
        free(prefix);
#endif

        /* swap current and next arrays */
#if defined(__cplusplus)
        std::swap(eq,   eq_n);
        std::swap(dist, dist_n);
#else
        { seq_t *tmp = eq;   eq   = eq_n;   eq_n   = tmp; }
        { seq_t *tmp = dist; dist = dist_n; dist_n = tmp; }
#endif

        cur_len = next_len;
    }

    /* sum all sequences of length n */
    seq_t result = 0;
    for (size_t v = 0; v < cur_len; ++v)
        result += eq[v] + dist[v];

#if !defined(__cplusplus)
    free(eq);
    free(dist);
    free(eq_n);
    free(dist_n);
#endif

    return result;
}
