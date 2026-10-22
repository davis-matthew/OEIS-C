/*
 * A379636  Triangle read by rows: T(n,k) (n >= k) is the maximum number of
 *          black cells in a n X k grid such that no black cell is
 *          edge-adjacent to more than one black cell.
 * https://oeis.org/A379636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 274, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   ceil2div3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A379636: triangle read by rows, T(n,k) gives
   a(n) for n≥1, where (row,col) is determined by triangular numbers.
   T(n,k) is computed by parity formulas from the OEIS entry. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ceil2div3.h"

static void find_row_col(idx_t n, idx_t *row, idx_t *col) {
    idx_t r = 1;
    idx_t t = 1;                      /* triangular number for current row */
    while (t < n) {
        r++;
        t += r;                       /* t = r*(r+1)/2 */
    }
    idx_t prev_t = t - r;
    *row = r;
    *col = n - prev_t;               /* 1 ≤ col ≤ row */
}

seq_t a(idx_t n) {
    if (n < 1) return 0;              /* undefined for n<1 */
    idx_t row, col;
    find_row_col(n, &row, &col);

    seq_t r = (seq_t)row;
    seq_t c = (seq_t)col;

    seq_t result;
    if ((row % 2 == 0) && (col % 2 == 0)) {                     /* both even */
        result = r * c / 2;
    } else if ((row % 2 == 0) && (col % 2 == 1)) {             /* even n, odd k */
        result = r * (c - 1) / 2 + ceil2div3(r);
    } else if ((row % 2 == 1) && (col % 2 == 0)) {             /* odd n, even k */
        result = c * (r - 1) / 2 + ceil2div3(c);
    } else {                                                   /* both odd */
        result = r * (c - 1) / 2 + ceil2div3(r);
    }
    return result;
}
