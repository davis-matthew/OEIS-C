/*
 * A254027  Table T(n,k) = 3^n - 2^k read by antidiagonals.
 * https://oeis.org/A254027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   read the table T(i,j)=3^i-2^j antidiagonally. a(n) = 3^k - 2^t,
 *            where d = ⌊(√(8n+1)-1)/2⌋, t = n - d(d+1)/2, k = d - t
 * Range:     i8 n <= 14, i16 n <= 54, i32 n <= 209, i64 n <= 819,
 *            i128 n <= 3320, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Find d such that d*(d+1)/2 ≤ n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    if (n != 0)
    {
        idx_t low = 0, high = n;
        while (low <= high)
        {
            idx_t mid = (low + high) >> 1;
            unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
            if (tri <= n)
            {
                d = mid;
                low = mid + 1;
            }
            else
            {
                if (mid == 0) break;
                high = mid - 1;
            }
        }
    }

    unsigned __int128 tri = (unsigned __int128)d * (d + 1) / 2;
    idx_t t = n - (idx_t)tri;   /* exponent of 2   */
    idx_t k = d - t;           /* exponent of 3   */

    seq_t pow_k = pow_uint(3, k);
    seq_t pow_t = pow_uint(2, t);
    return pow_k - pow_t;
}
