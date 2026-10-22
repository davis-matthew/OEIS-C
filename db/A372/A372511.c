/*
 * A372511  Number of solutions to x^2 + y^2 <= n, where x, y are positive odd
 *          integers.
 * https://oeis.org/A372511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of pairs of positive odd integers (x,y) such that x^2 + y^2 <= n */
#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* largest possible odd x */
    idx_t sqrt_n = (idx_t)floor_sqrt((seq_t)n);
    seq_t result = 0;

    for (idx_t x = 1; x <= sqrt_n; x += 2) {
        idx_t sq = x * x;            /* x^2 ≤ n */
        idx_t rem = n - sq;          /* remaining room for y^2 */

        if (rem < 1)                /* y must be positive odd integer */
            continue;

        idx_t m = (idx_t)floor_sqrt((seq_t)rem);
        seq_t cnt = (seq_t)(m + 1) / 2;   /* count of odd y ≤ m */
        result += cnt;
    }

    return result;
}
