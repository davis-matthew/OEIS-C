/*
 * A344373  a(n) = Sum_{k=1..n-1} (-1)^k gcd(k, n).
 * https://oeis.org/A344373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, look
 * Formula:   a(n) = Sum_{k=1..n-1} (-1)^k * gcd(k, n)
 * Range:     i8 n <= 63, i16 n <= 2879, i32 n <= 9430, i64 n <= 9430,
 *            i128 n <= 9430, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;               /* a(1) = 0 */
    seq_t sum = 0;
    for (idx_t k = 1; k < n; ++k)
    {
        seq_t g = gcd((seq_t)k, (seq_t)n);
        if (k & 1) sum -= g;            /* odd k: (-1)^k = -1 */
        else      sum += g;            /* even k: (-1)^k =  1 */
    }
    return sum;
}
