/*
 * A378849  a(n) is the total number of paths starting at (0,0), ending at
 *          (n,0), consisting of steps (1,1), (1,0), (1,-2), and staying on or
 *          above y = -1.
 * https://oeis.org/A378849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of paths from (0,0) to (n,0) with steps
 *            (1,1),(1,0),(1,-2) that never go below y = -1. The path is
 *            transformed by y' = y + 1, giving a walk with steps +1,0,-1 that
 *            starts and ends at height 1 and stays ≥ 0. The number of such
 *            walks is computed by dynamic programming over the cu...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utility prototypes are assumed to be available. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Special cases for offset 0. */
    if (n == 0)
        return 1;
    if (n == 1)
        return 1;
    if (n == 2)
        return 1;

    /* Arrays of heights: index = height + 1.  */
    size_t size = (size_t)n + 2;               /* allow heights up to n + 1 */
    seq_t *prev = (seq_t *)calloc(size, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(size, sizeof(seq_t));
    if (!prev || !curr)
        return 0;     /* in production code handle out of memory */

    prev[1] = 1;               /* height 0 at step 0 */

    for (idx_t step = 1; step <= n; ++step)
    {
        /* Zero out curr array for indices that will be written.  */
        memset(curr, 0, size * sizeof(seq_t));

        /* The maximum height after (step‑1) steps is (step‑1).  */
        idx_t max_h = step - 1;              /* height */
        idx_t max_idx = max_h + 1;           /* array index */

        for (idx_t idx = 0; idx <= max_idx; ++idx)
        {
            seq_t val = prev[idx];
            if (val == 0)
                continue;

            /* Step (1,1) : height +1  → idx+1 */
            curr[idx + 1] = curr[idx + 1] + val;

            /* Step (1,0) : height stays  → idx */
            curr[idx] = curr[idx] + val;

            /* Step (1,-2) : height -2  → idx-2  (possible only when idx>=2) */
            if (idx >= 2)
                curr[idx - 2] = curr[idx - 2] + val;
        }
        /* Prepare for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[1];   /* height 0 after n steps */
    free(prev);
    free(curr);
    return result;
}
