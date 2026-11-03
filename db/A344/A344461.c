/*
 * A344461  a(n) = Sum_{d|n} d^gcd(d,n/d).
 * https://oeis.org/A344461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^gcd(d,n/d)
 * Range:     u8 n <= 15, u16 n <= 48, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   gcd, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* sequence is defined for n >= 1 */
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t d1 = d;
            idx_t d2 = n / d;
            seq_t g = gcd((seq_t)d1, (seq_t)d2);        /* gcd(d1,d2) */
            idx_t exp = (idx_t)g;
            sum += ipow_u64_idx(d1, exp);
            if (d1 != d2)
                sum += ipow_u64_idx(d2, exp);                /* gcd(d2,d1) == g */
        }
    }
    return sum;
}
