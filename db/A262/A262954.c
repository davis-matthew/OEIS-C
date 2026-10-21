/*
 * A262954  Number of ordered pairs (x,y) with x >= 0 and y > 0 such that n -
 *          x^4 - y*(y+1)/2 has the form z^2 or 8*z^2.
 * https://oeis.org/A262954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x,y) | x ≥ 0, y > 0, * n - x^4 - y(y+1)/2 is a perfect
 *            square or an octagonally scaled square (8×z^2)}
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* sequence is defined for n ≥ 1 */
    seq_t res = 0;
    seq_t n_seq = (seq_t)n;
    for (idx_t x = 0; ; ++x) {
        seq_t x2 = (seq_t)x * (seq_t)x;
        seq_t x4 = x2 * x2;
        if (x4 > n_seq) break;                 /* x^4 cannot exceed n */
        seq_t rem = n_seq - x4;                /* remaining value after subtracting x^4 */
        for (idx_t y = 1; ; ++y) {
            seq_t ty = (seq_t)y * ((seq_t)y + 1) / 2;  /* triangular number T(y) */
            if (ty > rem) break;                /* further y will increase T(y) */
            seq_t m = rem - ty;                /* candidate value to test */

            /* check if m is a perfect square */
            seq_t sq = floor_sqrt(m);
            if (sq * sq == m) {
                res++;
                continue;
            }
            /* check if m is 8 × a perfect square */
            if ((m & 7) == 0) {                /* m divisible by 8 */
                seq_t m2 = m >> 3;
                seq_t sq2 = floor_sqrt(m2);
                if (sq2 * sq2 == m2) {
                    res++;
                }
            }
        }
    }
    return res;
}
