/*
 * A291171  Black numbers on the roulette wheel.
 * https://oeis.org/A291171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   The 18 black numbers on a roulette wheel in ascending order:
 *            a(1)=2, a(2)=4, a(3)=6, a(4)=8, a(5)=10, a(6)=11, a(7)=13,
 *            a(8)=15, a(9)=17, a(10)=20, a(11)=22, a(12)=24, a(13)=26,
 *            a(14)=28, a(15)=29, a(16)=31, a(17)=33, a(18)=35. (finite
 *            sequence)
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Return the n-th black number; defined only for 1 ≤ n ≤ 18. */
    static const seq_t blacks[18] = {
        2, 4, 6, 8, 10, 11, 13, 15,
        17, 20, 22, 24, 26, 28, 29, 31,
        33, 35
    };

    if (n == 0 || n > 18)
        return 0;
    return blacks[(idx_t)(n - 1)];
}
