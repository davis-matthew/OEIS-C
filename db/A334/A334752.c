/*
 * A334752  a(n) is the number immediately below n in a clockwise square
 *          spiral of the positive integers with the first step to the right.
 * https://oeis.org/A334752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number immediately below n in a clockwise square
 *            spiral of positive integers starting with a step to the right.
 *            Each element is found by converting n to its grid coordinate,
 *            moving one step down, and converting the new coordinate back to
 *            a number
 * Range:     u8 n <= 196, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9978 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: convert a coordinate (x,y) to its value in the spiral. */
static inline seq_t coord_to_value(long long x, long long y) {
    long long ax = x < 0 ? -x : x;
    long long ay = y < 0 ? -y : y;
    idx_t k = (ax > ay) ? (idx_t)ax : (idx_t)ay;
    if (k == 0) return (seq_t)1;

    idx_t n_start = (unsigned __int128)(2*k-1)*(2*k-1) + 1;
    idx_t offset = 0;

    if (x == (long long)k && y >= -(long long)k && y <= (long long)k-1) {
        /* south side */
        offset = (idx_t)((long long)k - 1 - y);
    } else if (y == -(long long)k && x >= -(long long)k && x <= (long long)k-1) {
        /* west side */
        offset = 2*k + (idx_t)((long long)k - 1 - x);
    } else if (x == -(long long)k && y >= -(long long)k+1 && y <= (long long)k) {
        /* north side */
        offset = 4*k + (idx_t)(y + (long long)k - 1);
    } else if (y == (long long)k && x >= -(long long)k+1 && x <= (long long)k) {
        /* east side */
        offset = 6*k + (idx_t)(x + (long long)k - 1);
    } else {
        /* This should not happen for valid spiral coordinates. */
        return (seq_t)0;
    }

    return (seq_t)(n_start + offset);
}

/* Helper: convert a value n (index) to its (x,y) coordinate in the spiral. */
static inline void value_to_coord(idx_t n, long long *x, long long *y) {
    if (n == 1) { *x = 0; *y = 0; return; }

    idx_t k = 0;
    while ((unsigned __int128)(2*k+1)*(2*k+1) < n) k++;

    idx_t n_start = (unsigned __int128)(2*k-1)*(2*k-1) + 1;
    idx_t offset  = n - n_start;
    idx_t side_len = 2*k;
    idx_t side = offset / side_len;
    idx_t pos  = offset % side_len;

    long long bx, by;
    switch (side) {
        case 0:   /* south side */
            bx = (long long)k;
            by = (long long)(k - 1 - pos);
            break;
        case 1:   /* west side */
            bx = (long long)(k - 1 - pos);
            by = -(long long)k;
            break;
        case 2:   /* north side */
            bx = -(long long)k;
            by = (long long)(-k + 1 + pos);
            break;
        default:  /* east side, side==3 */
            bx = (long long)(-k + 1 + pos);
            by = (long long)k;
            break;
    }
    *x = bx; *y = by;
}

/* Main sequence function */
seq_t a(idx_t n) {
    long long x, y;
    value_to_coord(n, &x, &y);          /* coordinates of n */
    return coord_to_value(x, y - 1);    /* number immediately below */
}
