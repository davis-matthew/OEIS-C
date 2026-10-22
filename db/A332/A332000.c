/*
 * A332000  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2 + (x + 47^2)^2 = y^2.
 * https://oeis.org/A332000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-5) - a(n-10) + 4418 for n >= 11, with a(1)=0,
 *            a(2)=752, a(3)=1820, a(4)=2231, a(5)=3995, a(6)=6627,
 *            a(7)=10575, a(8)=16511, a(9)=18840, a(10)=28952
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 43, u64 n <= 106, u128 n <= 232,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial 10 terms */
    static const seq_t init[10] = {
        0ULL,   752ULL,  1820ULL, 2231ULL, 3995ULL,
        6627ULL,10575ULL,16511ULL,18840ULL,28952ULL
    };

    if (n <= 10) return init[n-1];

    seq_t buf[10];
    for (int i = 0; i < 10; ++i) buf[i] = init[i];

    for (idx_t i = 11; i <= n; ++i) {
        int next_index = (int)((i-1) % 10);
        int index5     = (int)((i-5-1) % 10); /* a(i-5) */
        int index10    = (int)((i-10-1) % 10); /* a(i-10) */
        buf[next_index] = 6 * buf[index5] - buf[index10] + 4418;
    }

    return buf[(int)((n-1) % 10)];
}
