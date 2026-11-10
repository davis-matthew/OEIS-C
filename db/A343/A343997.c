/*
 * A343997  a(n) = A011772(n) if that number is even, otherwise A011772(n)+1.
 * https://oeis.org/A343997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A011772(n) if even, else A011772(n)+1; A011772(n) = least
 *            m>=1 with 2n | m(m+1), found by search over m < 2n
 * Range:     u8 n <= 127, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 97 terms through n = 9719 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    unsigned __int128 N = (unsigned __int128)n * 2u;
    idx_t m = 1;
    for (;; ++m) {
        unsigned __int128 p = (unsigned __int128)m * ((unsigned __int128)m + 1u);
        if (p % N == 0)
            break;
    }
    if (m & 1u)
        m++;
    return (seq_t)m;
}
