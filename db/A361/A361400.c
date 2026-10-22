/*
 * A361400  a(n) is the product of n and the number displayed on the upper
 *          face of a six-sided die as a result of its rotation through the
 *          edge when rolling over the cell with the number n of the square
 *          spiral of the natural row.
 * https://oeis.org/A361400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A361136(n) A361136(n) is the upper face of a standard
 *            die (opposite sides sum to 7, initial upper face 1) after
 *            rolling the die along the cells of the clockwise square spiral
 *            (Ulam spiral) from cell 1 up to cell n. This implementation
 *            simulates the die rotations in O(n) time
 * Range:     u8 n <= 42, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, behaviour for 0 is undefined */
    if (n == 1) return 1;          /* n * top = 1 * 1 */

    /* Die state: top, east, west, north, south, bottom.  */
    seq_t top   = 1;   /* initial upper face   */
    seq_t east  = 5;   /* opposite of west (2) */
    seq_t west  = 2;   /* opposite of east (5) */
    seq_t north = 4;   /* opposite of south(3) */
    seq_t south = 3;   /* opposite of north(4) */
    seq_t bottom = 6;  /* opposite of top (1)  */

    /* Spiral traversal state */
    idx_t dir   = 0;   /* 0=E, 1=N, 2=W, 3=S */
    idx_t len   = 1;   /* current leg length */
    idx_t steps = 0;   /* steps taken in the current leg */

    for (idx_t i = 2; i <= n; ++i) {
        /* Perform the roll according to current direction */
        switch (dir) {
            case 0: {               /* roll east (right)   */
                seq_t t = top, e = east, w = west, b = bottom;
                top   = w;
                east  = t;
                bottom= e;
                west  = b;
                break;
            }
            case 1: {               /* roll north (up)     */
                seq_t t = top, n = north, s = south, b = bottom;
                top   = s;
                north = t;
                bottom= n;
                south = b;
                break;
            }
            case 2: {               /* roll west (left)    */
                seq_t t = top, e = east, w = west, b = bottom;
                top   = e;
                west  = t;
                bottom= w;
                east  = b;
                break;
            }
            case 3: {               /* roll south (down)   */
                seq_t t = top, n = north, s = south, b = bottom;
                top   = n;
                south = t;
                bottom= s;
                north = b;
                break;
            }
        }

        /* Advance to next step in leg */
        ++steps;
        if (steps == len) {
            steps = 0;
            dir = (dir + 1) & 3;
            if ((dir & 1) == 0) {  /* increase leg length after 2 directions */
                ++len;
            }
        }
    }

    /* a(n) = n * top face value */
    return (seq_t)n * top;
}
