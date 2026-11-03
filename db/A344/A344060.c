/*
 * A344060  a(n) = Sum_{d|n} sigma(d)^n.
 * https://oeis.org/A344060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} sigma(d)^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities from the build environment */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* External linkage function computing the OEIS sequence A344060 */
seq_t a(idx_t n)
{
    /* n >= 1 by definition (offset 1) */
    seq_t result = 0;

    for (idx_t d = 1; d <= n; ++d)
    {
        if (n % d != 0) continue;           /* skip nondivisors */

        seq_t sigma_d = sigma_of(d);         /* sigma(d)  */
        seq_t power   = ipow(sigma_d, (seq_t)n); /* sigma(d)^n */

        result += power;                     /* accumulate sum */
    }

    return result;
}
