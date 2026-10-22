/*
 * A372644  For n >= 1, a(n) is the least k >= 1 such that for some x >= 1,
 *          n^2 - k^2 = x^3 or a(n) = -1 if no such k exists.
 * https://oeis.org/A372644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 64, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   floor_cube_root, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n): the least k≥1 such that n^2−k^2 = x^3 for some
    positive integer x; returns -1 if no such k exists.
    Algorithm: iterate over cubes ≤ n^2−1 starting from the largest,
    stop at first cube for which n^2−cube is a perfect square (k).  */
#include <stdbool.h>
#include "../utils/floor_cube_root.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < 1) return -1;                               // safety
    seq_t n2 = (seq_t)n * (seq_t)n;                    // n²

    seq_t max_cube_root = floor_cube_root(n2 - 1);      // largest x with x³ < n²
    if (max_cube_root == 0) return -1;                 // n=1

    idx_t xmax = (idx_t)max_cube_root;
    for (idx_t x = xmax; x >= 1; --x) {
        seq_t cube = (seq_t)x * (seq_t)x * (seq_t)x;    // x³
        seq_t diff = n2 - cube;                       // n² - x³ > 0
        seq_t r    = floor_sqrt(diff);                // ⌊√diff⌋
        if (r * r == diff) {                          // diff is a square
            return r;                                 // k = √diff
        }
    }
    return -1;                                         // no solution
}
