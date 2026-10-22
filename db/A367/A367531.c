/*
 * A367531  The number of ways of tiling the n X n grid up to 90-degree
 *          rotation by a tile that is fixed under 180-degree rotation but not
 *          90-degree rotation.
 * https://oeis.org/A367531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For n even (n = 2k): a(n) = 2^(k^2-1) + 2^(2k^2-2) + 2^(4k^2-2)
 *            For n odd (n = 2m-1): a(n) = 2^(2m^2-2m-1) + 2^(4m^2-4m-1)
 *            (where m = (n+1)/2, k = n/2). The implementation uses the helper
 *            function pow2(idx_t exp) from ../utils/pow2.h
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the special case n = 1 */
    if (n == 1)
        return (seq_t)1;

    if (n & 1) /* odd n = 2m-1 */
    {
        idx_t m = (n + 1) / 2;
        idx_t e1 = 2ull * m * m - 2ull * m - 1;
        idx_t e2 = 4ull * m * m - 4ull * m - 1;
        return pow2(e1) + pow2(e2);
    }
    else /* even n = 2k */
    {
        idx_t k = n / 2;
        idx_t e1 = k * k - 1;
        idx_t e2 = 2ull * k * k - 2;
        idx_t e3 = 4ull * k * k - 2;
        return pow2(e1) + pow2(e2) + pow2(e3);
    }
}
