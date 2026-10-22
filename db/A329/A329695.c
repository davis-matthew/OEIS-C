/*
 * A329695  Number of excursions of length n with Motzkin-steps avoiding the
 *          consecutive steps UD, HU and DH.
 * https://oeis.org/A329695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A329695: number of Motzkin excursions of length n
   avoiding consecutive steps  UD,  HU and  DH.
   The value is computed by dynamic programming over
   (altitude, last step type).  Initial values:
   a(0)=1, a(1)=1, a(2)=1.
   The algorithm runs in O(n^2) time with a table of size (n+1)*3. */

#ifdef __cplusplus
# include <vector>
# include <cstddef>
# else
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# endif

extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* the maximal altitude after n steps is n */
    size_t const size = (size_t)(n + 1) * 3;

#ifdef __cplusplus
    std::vector<seq_t> cur_vec(size);
    std::vector<seq_t> nxt_vec(size);
    seq_t *cur = cur_vec.data();
    seq_t *nxt = nxt_vec.data();
#else
    seq_t *cur = malloc(size * sizeof(seq_t));
    seq_t *nxt = malloc(size * sizeof(seq_t));
    if (!cur || !nxt) { /* allocation failed */
        free(cur); free(nxt);
        return (seq_t)0;
    }
    /* zero‑initialise all entries */
    memset(cur, 0, size * sizeof(seq_t));
    memset(nxt, 0, size * sizeof(seq_t));
#endif

    /* --- start after the first step (length = 1) --- */
    cur[0 * 3 + 1] = (seq_t)1;          /* horizontal step at altitude 0 */
    cur[1 * 3 + 0] = (seq_t)1;          /* up step to altitude 1 */

    /* --- walk from step 1 up to n-1 --- */
    for (idx_t step = 1; step < n; ++step) {
        /* zero all entries of nxt */
#ifdef __cplusplus
        for (size_t i = 0; i < size; ++i) nxt[i] = (seq_t)0;
#else
        memset(nxt, 0, size * sizeof(seq_t));
#endif

        /* iterate over all states reached after 'step' steps */
        for (idx_t h = 0; h <= step; ++h) {
            seq_t *c = cur + h * 3;          /* pointers to U/H/D for this altitude */

            for (int last = 0; last < 3; ++last) {
                seq_t val = c[last];
                if (val == (seq_t)0) continue;

                /* 1) place an Up step (U) - forbidden after H (last==1) */
                if (last != 1) {
                    nxt[(h + 1) * 3 + 0] += val;
                }

                /* 2) place a Horizontal step (H) - forbidden after D (last==2) */
                if (last != 2) {
                    nxt[h * 3 + 1] += val;
                }

                /* 3) place a Down step (D) - requires h>0 and not after U (last==0) */
                if (h > 0 && last != 0) {
                    nxt[(h - 1) * 3 + 2] += val;
                }
            }
        }

        /* swap buffers for next iteration */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* final result: end at altitude 0 (any last step type) */
    seq_t result = cur[0 * 3 + 0] + cur[0 * 3 + 1] + cur[0 * 3 + 2];

#ifndef __cplusplus
    free(cur);
    free(nxt);
#endif
    return result;
}
