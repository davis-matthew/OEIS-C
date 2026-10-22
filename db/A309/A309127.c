/*
 * A309127  a(n) = n + 2^4 * floor(n/2^4) + 3^4 * floor(n/3^4) + 4^4 *
 *          floor(n/4^4) + ...
 * https://oeis.org/A309127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + Σ_{k=2}^{⌊n^{1/4}⌋} k^4 ⋅ floor(n / k^4)
 * Range:     u8 n <= 94, u16 n <= 8124, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for zero, though offset starts at 1 */

    seq_t result = (seq_t)n;               /* start with the n term */

    for (idx_t k = 2; ; ++k) {
        /* compute k^4 carefully; all intermediate products fit in 64 bits
           because k^4 ≤ n ≤ 2^64−1 implies k ≤ 65535 */
        idx_t k4 = (idx_t)k * k * k * k;
        if (k4 > n) break;

        idx_t floor_n = n / k4;           /* integer division */
        result += (seq_t)k4 * (seq_t)floor_n;
    }

    return result;
}
