/*
 * A296030  Pairs of coordinates for successive integers in the square spiral
 *          (counterclockwise).
 * https://oeis.org/A296030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, look
 * Formula:   not stated
 * Range:     i8 n <= 9782, i16 n <= 9782, i32 n <= 9782, i64 n <= 9782,
 *            i128 n <= 9782, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296030:  pairs of coordinates for successive integers in the
   square spiral (counterclockwise).  
   a(2m-1) = x-coordinate of integer m,   a(2m) = y-coordinate of integer m
   where (x(m),y(m)) is the position of m in the spiral starting at (0,0)
   and moving right, up, left, left, down, ... */
/* Implementation details:
   * uses integer arithmetic only
   * computes the layer k of the ring, then the offset along that ring
   * returns the appropriate coordinate depending on the parity of the index
   * provides an O(1) version of integer square root for 64‑bit indices
   * works with any seq_t width that can hold the values
*/

#include <stdint.h>
#include <stddef.h>

/* integer square root: floor(sqrt(n)) for 0 <= n < 2^64 */
static idx_t isqrt(idx_t n) {
    if (n == 0) return 0;
    idx_t low = 0, high = n;
    while (high - low > 1) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * mid <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* compute (x,y) coordinates of integer pos (1‑based) in the counter‑clockwise
   square spiral. pos >= 1. */
static void xy_of_pos(idx_t pos, seq_t *x, seq_t *y) {
    if (pos == 1) {            /* the origin */
        *x = 0;
        *y = 0;
        return;
    }
    /* layer k: numbers from (2k-1)^2+1 to (2k+1)^2 */
    idx_t k = (isqrt(pos - 1) + 1) / 2;   /* layer index */
    idx_t side = 2 * k;                    /* side length of the square ring */

    /* previous square: (2(k-1)+1)^2  */
    idx_t prev_side = 2 * (k - 1) + 1;
    idx_t prev_square = prev_side * prev_side;

    idx_t start = prev_square + 1;         /* first value in this ring */
    idx_t offset = pos - start;            /* 0 .. 4*side-1 */

    seq_t kt = (seq_t)k;
    seq_t y0 = -(seq_t)(k - 1);            /* y of the starting point */

    if (offset < side) {                   /* moving up */
        *x = kt;
        *y = y0 + (seq_t)offset;
    } else if (offset < 2 * side) {        /* moving left */
        idx_t off = offset - side;
        *x = kt - (seq_t)(off + 1);
        *y = y0 + (seq_t)(side - 1);
    } else if (offset < 3 * side) {        /* moving down */
        idx_t off = offset - 2 * side;
        *x = -kt;
        *y = kt - (seq_t)(off + 1);
    } else {                               /* moving right */
        idx_t off = offset - 3 * side;
        *x = -kt + (seq_t)(off + 1);
        *y = -kt;
    }
}

/* Main sequence function: a(n).  n >= 1.  a(2m-1) = x(m), a(2m) = y(m). */
seq_t a(idx_t n) {
    if (n & 1U) {                          /* odd index */
        idx_t m = (n + 1) / 2;
        seq_t x, y;
        xy_of_pos(m, &x, &y);
        return x;
    } else {                               /* even index */
        idx_t m = n / 2;
        seq_t x, y;
        xy_of_pos(m, &x, &y);
        return y;
    }
}
