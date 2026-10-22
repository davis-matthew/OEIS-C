/*
 * A321555  a(n) = Sum_{d|n} (-1)^(n/d+1)*d^10.
 * https://oeis.org/A321555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^(n/d+1)*d^10
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 84, u128 n <= 7132,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n>=1; for n==0 return 0. */
    if (n == 0) return 0;

    seq_t pos = 0;
    seq_t neg = 0;

    /* Enumerate all divisors d of n. */
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == 0) {
            idx_t t = n / d;                 /* t = n/d */
            bool posSign = (t & 1) != 0;      /* sign positive if t is odd */

            seq_t term = pow_uint((uint64_t)d, 10); /* d^10 */

            if (posSign)
                pos += term;
            else
                neg += term;
        }
    }

    /* The sum is always non‑negative for all n. */
    return pos - neg;
}
