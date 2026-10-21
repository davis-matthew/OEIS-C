/*
 * A259346  If n = 2^k then a(n) = 3^k, otherwise a(n) = 0.
 * https://oeis.org/A259346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 3^k if n = 2^k, otherwise 0
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                     /* Undefined for n=0, return 0 */
    }
    if ((n & (n - 1)) != 0) {          /* Not a power of two */
        return 0;
    }
    idx_t k = 0;
    idx_t t = n;
    while (t > 1) {                    /* Count bits to get exponent k */
        t >>= 1;
        ++k;
    }
    return pow3(k);                    /* Return 3^k */
}
