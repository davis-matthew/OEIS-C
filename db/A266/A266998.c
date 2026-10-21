/*
 * A266998  Number of iterations of the map "n -> sum of the triangular
 *          numbers whose indices are the digits of n" needed to reach 1.
 * https://oeis.org/A266998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of iterations to reach 1 in the map f(n) = sum of
 *            triangular numbers of the decimal digits of n
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    static const seq_t T[10] = {0,1,3,6,10,15,21,28,36,45};
    seq_t steps = 0;
    if (n == 1) return 0;
    while (n != 1)
    {
        seq_t s = 0;
        uint64_t tmp = n;
        while (tmp > 0)
        {
            unsigned d = tmp % 10;
            s += T[d];
            tmp /= 10;
        }
        n = (idx_t)s;
        steps++;
    }
    return steps;
}
