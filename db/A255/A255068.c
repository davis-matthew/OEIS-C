/*
 * A255068  a(n) is the largest k such that A255070(k) = n.
 * https://oeis.org/A255068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A091067(n+1) - 1, where A091067(m) is the m-th integer
 *            whose odd part is == 3 (mod 4); computed by a linear search
 * Range:     u8 n <= 126, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 89 terms through n = 7448 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t candidate = 1;
    idx_t found = 0;
    idx_t target = n + 1;

    while (found < target) {
        candidate++;
        idx_t x = candidate;
        while ((x & 1) == 0)
            x >>= 1;
        if ((x & 3) == 3)
            ++found;
    }
    return (seq_t)(candidate - 1);
}
