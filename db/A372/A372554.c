/*
 * A372554  a(n) = A050602(n, 2n+1).
 * https://oeis.org/A372554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A050602(n, 2n+1), where A(x,y) = 0 if x AND y = 0, else 1
 *            + A(x XOR y, 2*(x AND y)); computed by iterating the carry
 *            recurrence
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 122 terms through n = 9645 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A050602_calc(unsigned __int128 x, unsigned __int128 y)
{
    seq_t cnt = 0;
    while ((x & y) != 0) {
        cnt++;
        unsigned __int128 nx = x ^ y;
        unsigned __int128 ny = (x & y) << 1;
        x = nx;
        y = ny;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    unsigned __int128 x = (unsigned __int128)n;
    unsigned __int128 y = (unsigned __int128)n * 2 + 1;
    return A050602_calc(x, y);
}
