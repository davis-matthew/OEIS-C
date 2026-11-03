/*
 * A270469  Number of ordered ways to write n = x^3 + y*(y+1) + z*(3*z+2),
 *          where x and y are nonnegative integers and z is a nonzero integer.
 * https://oeis.org/A270469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count of ordered triples (x,y,z) with * x^3 + y(y+1) + z(3z+2) =
 *            n, * where x, y are nonnegative integers, * and z is a nonzero
 *            integer. * The function is indexed like the OEIS entry: a(1)
 *            corresponds to n = 1
 * Range:     u8 n <= 4, u16 n <= 1148, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9817 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* Undefined for n < 1, return 0 for safety */

    seq_t count = 0;

    for (idx_t x = 0;; ++x) {
        idx_t x3 = (idx_t)x * x * x;
        if (x3 > n) break;

        for (idx_t y = 0;; ++y) {
            idx_t yym = y * (y + 1);
            if (x3 + yym > n) break;

            idx_t R = n - x3 - yym;
            if (R == 0) continue;        /* z cannot be 0, C > 0 */

            /* Solve 3z^2 + 2z = R  ->  D = 4 + 12R must be a perfect square */
            seq_t D = (seq_t)4 + (seq_t)12 * (seq_t)R;
            seq_t sqrtD = floor_sqrt(D);
            if (sqrtD * sqrtD == D) {
                /* For D ≡ 2 (mod 6) (sqrtD % 6 == 2) we get positive z,
                 * for D ≡ 4 (mod 6) (sqrtD % 6 == 4) we get negative z.
                 * In both cases z ≠ 0 whenever D is a perfect square > 4,
                 * which holds for all R > 0. */
                count++;
            }
        }
    }

    return count;
}
