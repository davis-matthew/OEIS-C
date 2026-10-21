/*
 * A287731  Bisection of A287729.
 * https://oeis.org/A287731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A287729(2*n-1) computed via the binary recursion
 *            described in the OEIS entry
 * Range:     u8 n <= 6441, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* n >= 1 */
    if (n <= 1)
        return (seq_t)1;

    idx_t m = n - 1;                     /* n-1 */
    idx_t t = floor_log2_uint64(m);      /* number of bits minus 1 */

    seq_t c = 1;
    seq_t s = 1;

    for (idx_t i = 0; i < t; ++i) {
        idx_t bit = (m >> i) & 1;        /* process from least significant bit */
        if (bit == (t & 1))
            s += c;
        else
            c += s;
    }
    return c;
}
