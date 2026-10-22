/*
 * A317612  For k >= 1, fill a k X k square with the numbers 1 to k^2 by rows
 *          left to right and top to bottom; then read the square by a square
 *          clockwise spiral beginning at the top left and spiraling inwards.
 * https://oeis.org/A317612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th element of the infinite sequence obtained by
 *            concatenating the clockwise spiral orders of the k×k matrices (k
 *            = 1, 2, 3, …) whose entries are filled row‑wise with 1 … k².
 *            a(1) = 1
 * Range:     u8 n <= 110, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Determine the size k of the square that contains the n-th element. */
    idx_t sum = 0;        /* cumulative number of elements up to current k */
    idx_t k   = 0;
    while (1) {
        ++k;
        sum += (idx_t)k * k;
        if (n <= sum)
            break;
    }

    /* Position of n within the k×k block (0‑based). */
    idx_t prevSum = sum - (idx_t)k * k;
    idx_t pos     = n - prevSum - 1;

    /* Find the layer and the (row, col) coordinates of pos in the spiral. */
    idx_t layer = 0;
    idx_t r = 0, c = 0;
    while (1) {
        idx_t s = k - 2 * layer;      /* side length of current layer */
        if (s == 1) {                 /* center cell of an odd‑sized square */
            r = layer;
            c = layer;
            break;
        }

        idx_t perim = 4 * (s - 1);     /* number of cells in this layer */
        if (pos < perim) {
            /* pos lies within this layer */
            idx_t off = pos;

            /* Top edge (left → right). */
            if (off < s) {
                r = layer;
                c = layer + off;
                break;
            }
            off -= s;

            /* Right edge (top+1 → bottom). */
            if (off < s - 1) {
                r = layer + 1 + off;
                c = layer + s - 1;
                break;
            }
            off -= (s - 1);

            /* Bottom edge (right‑1 → left). */
            if (off < s - 1) {
                r = layer + s - 1;
                c = layer + s - 2 - off;
                break;
            }
            off -= (s - 1);

            /* Left edge (bottom‑1 → top+1). */
            /* For s == 2 the left edge has 0 cells; this case never
               reaches here because off would be 0 after the bottom edge. */
            r = layer + s - 2 - off;
            c = layer;
            break;
        } else {
            /* Skip over this layer and continue with the next. */
            pos -= perim;
            ++layer;
        }
    }

    /* Convert the (r, c) coordinate to the row‑major value in the k×k matrix. */
    seq_t val = (seq_t)r * (seq_t)k + (seq_t)c + (seq_t)1;
    return val;
}
