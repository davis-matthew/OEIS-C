/*
 * A372512  Number of solutions to x^2 + y^2 + z^2 <= n, where x, y, z are
 *          positive odd integers.
 * https://oeis.org/A372512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A372512
   Formula: a(n) = # { (x,y,z) | x,y,z positive odd integers,
                              x^2 + y^2 + z^2 <= n } */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t count = 0;

    /* Maximum odd integer whose square does not exceed n */
    idx_t sqrt_n   = (idx_t)floor_sqrt((seq_t)n);
    idx_t odd_cnt  = (sqrt_n + 1) / 2;   /* number of odds 1,3,…,sqrt_n */

    for (idx_t i = 0; i < odd_cnt; ++i) {
        seq_t x = (seq_t)(2 * i + 1);
        seq_t x_sq = x * x;

        for (idx_t j = 0; j < odd_cnt; ++j) {
            seq_t y = (seq_t)(2 * j + 1);
            seq_t y_sq = y * y;

            seq_t sum_xy = x_sq + y_sq;
            if (sum_xy > (seq_t)n)          /* further y increase will only grow */
                break;

            seq_t remaining = (seq_t)n - sum_xy;
            idx_t sqrt_rem  = (idx_t)floor_sqrt(remaining);
            seq_t z_cnt     = (seq_t)((sqrt_rem + 1) / 2);   /* number of odd z ≤ √rem */

            count += z_cnt;
        }
    }
    return count;
}
