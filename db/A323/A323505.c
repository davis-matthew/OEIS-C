/*
 * A323505  Mirror image of (denominators of) Bernoulli tree, A106831.
 * https://oeis.org/A323505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(0) = 1; a(n) = 2*a(n/2) for even n > 0; for odd n, a(n) =
 *            (A001511(n+1)+1-A036987(n)) * a((n-1)/2)
 * Range:     u8 n <= 30, u16 n <= 190, u32 n <= 4094, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 91 terms through n = 9960 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    while (n > 0) {
        if ((n & 1ULL) == 0ULL) {
            result *= (seq_t)2;
            n >>= 1;
        } else {
            idx_t x = n + 1;
            if (x == 0) {
                /* n = 2^64-1: n+1 = 2^64, a power of two with ruler value 65 */
                result *= (seq_t)(65 + 1 - 1);
            } else {
                unsigned r = 1;
                idx_t y = x;
                while ((y & 1ULL) == 0ULL) {
                    r++;
                    y >>= 1;
                }
                unsigned f = ((x & (x - 1)) == 0ULL) ? 1u : 0u;
                result *= (seq_t)(r + 1 - f);
            }
            n = (n - 1) >> 1;
        }
    }
    return result;
}
