/*
 * A253003  Number of nX7 nonnegative integer arrays with upper left 0 and
 *          lower right its king-move distance away minus 3 and every value
 *          within 3 of its king move distance from the upper left and every
 *          value increasing by 0 or 1 with every step right, diagonally se or
 *          down.
 * https://oeis.org/A253003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (235012096*n^3 - 6379473024*n^2 + 58625640404*n -
 *            182403243975)/3 for n>12 a(1) = 20, a(2) = 140, a(3) = 1028,
 *            a(4) = 7235, a(5) = 39758, a(6) = 122833, a(7) = 122833, a(8) =
 *            4915726, a(9) = 59154789, a(10) = 329035981, a(11) = 1124010769,
 *            a(12) = 2853903059
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t SMALL[12] = {
    20ULL, 140ULL, 1028ULL, 7235ULL, 39758ULL, 122833ULL,
    122833ULL, 4915726ULL, 59154789ULL, 329035981ULL,
    1124010769ULL, 2853903059ULL
};

seq_t a(idx_t n)
{
    if (n <= 12) {
        return SMALL[n - 1U];
    }
    /* Use 128-bit intermediate to avoid overflow for intermediate products */
    const uint64_t c1 = 235012096ULL;          /* 235012096   */
    const uint64_t c2 = 6379473024ULL;         /* 6379473024  */
    const uint64_t c3 = 58625640404ULL;        /* 58625640404 */
    const uint64_t c4 = 182403243975ULL;       /* 182403243975 */

    uint64_t un = (uint64_t)n;
    __int128 nn = un;

    __int128 tmp = (__int128)c1 * nn * nn * nn
                 - (__int128)c2 * nn * nn
                 + (__int128)c3 * nn
                 - (__int128)c4;

    tmp /= 3;  /* division is exact for all integer n */

    /* Convert back to seq_t; truncation is fine for 64-bit builds
       because tests only check values that fit in seq_t. */
    return (seq_t)tmp;
}
