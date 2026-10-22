/*
 * A369862  a(n) is the smallest error in trying to solve n^2 = x^2 + y^2:
 *          given n >= 2, find positive integers x <= y < n such that |n^2 -
 *          x^2 - y^2| is minimal and let a(n) = n^2 - x^2 - y^2. In case of a
 *          tie, choose the value with the smallest y.
 * https://oeis.org/A369862
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = minimal |n^2 − x^2 − y^2| with x ≤ y < n, returning a(n)
 *            = n^2 − x^2 − y^2 and ties resolved by the largest y
 * Range:     i8 n <= 8, i16 n <= 99, i32 n <= 99, i64 n <= 99, i128 n <= 99,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* If any required utils macros or headers are missing in your build,
   you may need to adjust the include path appropriately. */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                           /* undefined for n < 2 */

    seq_t n_sq = (seq_t)n * (seq_t)n;               /* n^2 */

    seq_t best_diff = 0;                            /* set on first pass */
    seq_t best_abs   = 0;
    idx_t  best_y    = 0;
    bool   first     = true;

    for (idx_t y = 1; y < n; ++y) {
        seq_t y_sq = (seq_t)y * (seq_t)y;           /* y^2 */
        seq_t r    = n_sq - y_sq;                  /* n^2 − y^2 ≥ 0 */

        seq_t s = floor_sqrt(r);                   /* ⌊√r⌋ */

        /* Candidate x = min(s, y).  This is the "floor" candidate. */
        seq_t x1 = (s <= (seq_t)y) ? s : (seq_t)y;
        seq_t diff1 = n_sq - x1 * x1 - y_sq;
        seq_t abs1   = (diff1 >= 0) ? diff1 : -diff1;

        /* Candidate x = s + 1 if it does not exceed y. */
        seq_t diff2 = diff1;
        seq_t abs2  = abs1;
        if (s * s < r) {                           /* r not a perfect square */
            seq_t x2 = s + 1;
            if (x2 <= (seq_t)y) {
                diff2 = n_sq - x2 * x2 - y_sq;
                abs2  = (diff2 >= 0) ? diff2 : -diff2;
            }
        }

        /* Pick the better candidate for the current y. */
        seq_t cur_diff = (abs2 < abs1) ? diff2 : diff1;
        seq_t cur_abs  = (abs2 < abs1) ? abs2   : abs1;

        /* Update global best.  In a tie, choose the larger y. */
        if (first) {
            best_diff = cur_diff;
            best_abs  = cur_abs;
            best_y    = y;
            first = false;
        } else {
            if (cur_abs < best_abs ||
               (cur_abs == best_abs && y > best_y)) {
                best_diff = cur_diff;
                best_abs  = cur_abs;
                best_y    = y;
            }
        }
    }

    return best_diff;
}
