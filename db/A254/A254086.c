/*
 * A254086  Number of (n+2)X(2+2) 0..1 arrays with every 3X3 subblock diagonal
 *          maximum minus antidiagonal maximum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A254086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The function counts binary (n+2)×4 arrays whose 3×3 subblocks
 *            satisfy the * monotonic “main‑diagonal maximum minus
 *            anti‑diagonal maximum” condition. * It uses a dynamic
 *            programming approach over rows: the state stores the * previous
 *            diff pair (for the last completed subblock row) and the last *
 *            t...
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   compute_diff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/compute_diff.h"

#ifdef __cplusplus
#define ALLOCATE_SEQ_ARRAY(n)  new seq_t[(n)]()
#define FREE_SEQ_ARRAY(p)      delete[] (p)
#else
#define ALLOCATE_SEQ_ARRAY(n)  ((seq_t*)calloc((n), sizeof(seq_t)))
#define FREE_SEQ_ARRAY(p)      free((void*)(p))
#endif

seq_t a (idx_t n)
{
    if (n < 1) return 0;           /* offset 1  */
    idx_t H = n + 2;                /* number of rows */

    const int STATES = 9 * 256;     /* 3 diff1 × 3 diff2 × 16 × 16 */
    seq_t *curr = ALLOCATE_SEQ_ARRAY(STATES);
    seq_t *next = ALLOCATE_SEQ_ARRAY(STATES);

    /* initial state: rows 1,2,3 */
    for (int a = 0; a < 16; ++a)
    for (int b = 0; b < 16; ++b)
    for (int c = 0; c < 16; ++c)
    {
        int d0, d1;
        compute_diff (a, b, c, &d0, &d1);
        if (d0 <= d1)                /* horizontal monotonicity */
        {
            int idx = ((d0 + 1) * 3 + (d1 + 1)) * 256 + b * 16 + c;
            curr[idx] += 1;
        }
    }

    /* process remaining rows 4 … H */
    for (idx_t row = 4; row <= H; ++row)
    {
        /* clear next */
        for (int i = 0; i < STATES; ++i) next[i] = 0;

        for (int s = 0; s < STATES; ++s)
        {
            seq_t count = curr[s];
            if (!count) continue;

            /* decode state */
            int t = s / 256;
            int rem = s % 256;
            int diffPrev1 = (t / 3) - 1;   /* -1,0,1 */
            int diffPrev2 = (t % 3) - 1;   /* -1,0,1 */
            int lastRow1 = rem / 16;       /* row r-1 */
            int lastRow2 = rem % 16;       /* row r   */

            for (int newRow = 0; newRow < 16; ++newRow)
            {
                int d0, d1;
                compute_diff (lastRow1, lastRow2, newRow, &d0, &d1);

                /* monotonicity checks */
                if (diffPrev1 <= d0 &&
                    diffPrev2 <= d0 &&
                    diffPrev2 <= d1 &&
                    d0       <= d1)
                {
                    int idx = ((d0 + 1) * 3 + (d1 + 1)) * 256 + lastRow2 * 16 + newRow;
                    next[idx] += count;
                }
            }
        }

        /* swap */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;
    }

    /* sum all remaining states */
    seq_t total = 0;
    for (int i = 0; i < STATES; ++i) total += curr[i];

    FREE_SEQ_ARRAY(curr);
    FREE_SEQ_ARRAY(next);
    return total;
}
