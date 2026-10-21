/*
 * A282023  Start with 1; multiply alternately by 4 and 3.
 * https://oeis.org/A282023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(1) = 4; a(n) = 12*a(n-2) for n > 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 4;
    if ((n & 1) == 0) {
        /* even n: a(n) = 2^n * 3^(n/2) */
        return pow2(n) * pow3(n / 2);
    } else {
        /* odd n: a(n) = 2^(n+1) * 3^((n-1)/2) */
        return pow2(n + 1) * pow3((n - 1) / 2);
    }
}
