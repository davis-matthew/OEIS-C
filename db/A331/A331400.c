/*
 * A331400  The grid points visible from the central point of an infinite 2D
 *          square lattice where all grid points are numbered as in the Ulam
 *          spiral.
 * https://oeis.org/A331400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th spiral label that is visible from the center,
 *            i.e. its integer coordinates (x,y) satisfy gcd(|x|,|y|)=1
 * Range:     u8 n <= 159, u16 n <= 9483, u32 n <= 9483, u64 n <= 9483,
 *            u128 n <= 9483, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 0 */

    /* Spiral iteration state */
    idx_t cur_layer = 1;                 /* current layer index   */
    idx_t prev_end  = 1;                 /* last number of previous layer */
    idx_t layer_end = 9;                 /* last number of current layer  */
    idx_t num       = 2;                 /* next spiral label to examine */

    idx_t visible_cnt = 0;               /* how many visible points found */
    seq_t result = 0;                    /* value to return when counter reaches n */

    while (visible_cnt < n)
    {
        /* Move to next layer if we have crossed the current layer's end. */
        if (num > layer_end)
        {
            cur_layer++;
            prev_end = layer_end;
            /* (2*cur_layer+1)^2 fits in 64‑bit for the required n. */
            layer_end = (2 * cur_layer + 1) * (2 * cur_layer + 1);
        }

        /* Offset of the current number within the layer (0‑based). */
        idx_t offset = num - prev_end - 1;

        /* Compute the integer coordinates (x,y) of the current label. */
        int64_t k = (int64_t)cur_layer;
        int64_t x, y;

        if (offset < 2 * k)                      /* right side, moving up   */
        {
            x = k;
            y = -k + 1 + (int64_t)offset;
        }
        else if (offset < 4 * k)                 /* top side, moving left   */
        {
            int64_t off = (int64_t)offset - 2 * k;
            x = k - 1 - off;
            y = k;
        }
        else if (offset < 6 * k)                 /* left side, moving down  */
        {
            int64_t off = (int64_t)offset - 4 * k;
            x = -k;
            y = k - 1 - off;
        }
        else                                     /* bottom side, moving right*/
        {
            int64_t off = (int64_t)offset - 6 * k;
            x = -k + 1 + off;
            y = -k;
        }

        /* Check visibility: gcd(|x|,|y|) == 1. */
        seq_t ax = (seq_t)(x >= 0 ? x : -x);
        seq_t ay = (seq_t)(y >= 0 ? y : -y);
        if (gcd(ax, ay) == (seq_t)1)
        {
            visible_cnt++;
            if (visible_cnt == n)
            {
                result = (seq_t)num;
                break;
            }
        }

        num++;   /* advance to the next label in the spiral */
    }

    return result;
}
