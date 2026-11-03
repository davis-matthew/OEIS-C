/*
 * A321294  a(n) = Sum_{d|n} mu(n/d)*d*sigma_n(d).
 * https://oeis.org/A321294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} φ(n/d) · d^(n+1) (offset = 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 385 (2026-11-01)
 * Helpers:   ipow_idx, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; we return 0 for n=0 just to be safe. */
    if (n == 0) return 0;

    seq_t sum = 0;
    idx_t  nn  = n;          /* local copy to ease divisor loop */
    idx_t  exp = n + 1;      /* exponent n+1 */

    /* Iterate over divisors d of n. */
    for (idx_t d = 1; d * d <= nn; ++d)
    {
        if (nn % d != 0) continue;   /* d is not a divisor */

        idx_t d1 = d;          /* divisor d  */
        idx_t d2 = nn / d;     /* complementary divisor n/d */

        /* term for divisor d1 : φ(n/d1) · d1^(n+1) */
        seq_t phi1 = totient(d2);                    /* φ(n/d1)   */
        seq_t pow1 = ipow_idx((seq_t)d1, exp);    /* d1^(n+1)  */
        sum += phi1 * pow1;

        if (d1 != d2)
        {
            /* term for divisor d2 : φ(n/d2) · d2^(n+1) */
            seq_t phi2 = totient(d1);                /* φ(n/d2)   */
            seq_t pow2 = ipow_idx((seq_t)d2, exp);/* d2^(n+1)  */
            sum += phi2 * pow2;
        }
    }

    return sum;
}
