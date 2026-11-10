/*
 * A287568  Positions of 1 in A287566.
 * https://oeis.org/A287568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4n - A287566(n)
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9651 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const unsigned char _A287568_map[4][4] = {
    {0, 2, 3, 1},
    {2, 3, 1, 0},
    {3, 1, 0, 2},
    {1, 0, 2, 3}
};

static seq_t A287566_a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t i = n - 1;
    unsigned char s = 0;

    if (i == 0)
        return (seq_t)s;

    idx_t pw = 1;
    while (pw <= i / 4)
        pw <<= 2;

    while (pw > 0) {
        unsigned char child = (unsigned char)(i / pw);
        i %= pw;
        s = _A287568_map[s][child];
        pw >>= 2;
    }

    return (seq_t)s;
}

seq_t a(idx_t n)
{
    return (seq_t)n * 4 - A287566_a(n);
}
