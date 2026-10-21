/*
 * A257386  Number of Motzkin paths of length n with no level steps at height
 *          3.
 * https://oeis.org/A257386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Motzkin paths of length n with no level steps
 *            at height 3. The value is computed by a dynamic‑programming
 *            procedure over the path height. For each step we keep the number
 *            of paths ending at each possible height, forbidding a horizontal
 *            step when the height equals 3. Complexity O(...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 47, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* allocate two working arrays */
#ifdef __cplusplus
    seq_t *dp_cur = new seq_t[n + 2];
    seq_t *dp_next = new seq_t[n + 2];
#else
    seq_t *dp_cur  = (seq_t*)calloc(n + 2, sizeof(seq_t));
    seq_t *dp_next = (seq_t*)calloc(n + 2, sizeof(seq_t));
#endif

    /* initialise the empty path */
    dp_cur[0] = 1;

    for (idx_t step = 0; step < n; ++step) {
        /* clear next array entries that can be written */
        for (idx_t h = 0; h <= step + 1; ++h)
            dp_next[h] = 0;

        for (idx_t h = 0; h <= step; ++h) {
            seq_t val = dp_cur[h];
            if (val == 0) continue;

            /* horizontal step (disallowed at height 3) */
            if (h != 3)
                dp_next[h] += val;

            /* up step */
            dp_next[h + 1] += val;

            /* down step, only if current height > 0 */
            if (h > 0)
                dp_next[h - 1] += val;
        }

        /* swap dp_cur and dp_next for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* the answer is the number of paths ending at height 0 */
    seq_t result = dp_cur[0];

    /* free memory */
#ifdef __cplusplus
    delete[] dp_cur;
    delete[] dp_next;
#else
    free(dp_cur);
    free(dp_next);
#endif

    return result;
}
