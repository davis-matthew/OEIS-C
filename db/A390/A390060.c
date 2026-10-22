/*
 * A390060  E.g.f. A(x) satisfies A(x) = exp( x * (1+x^2)^2 * A(x)^2 ).
 * https://oeis.org/A390060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/2⌋} (2*(n-2k)+1)^(n-2k-1) *
 *            binomial(2*(n-2k),k) / (n-2k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   _binom_local, _pow_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_binom_local.h"
#include "../utils/_pow_local.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    seq_t sum   = 0;
    seq_t ratio = 1;                       /* n!/(n-2k)! for current k */
    idx_t m = n;                           /* m = n-2k */

    for (idx_t k = 0; k <= n / 2; ++k) {
        if (m == 0) break;                /* term vanishes for m==0 */

        seq_t power = _pow_local((seq_t)(2 * m + 1), m - 1);
        seq_t b = _binom_local(2 * m, k);
        seq_t term = ratio * b;
        term = term * power;
        sum = sum + term;

        /* Update ratio for next larger k (i.e., m decreases by 2). */
        if (m > 1)
            ratio = ratio * (seq_t)(m - 1) * (seq_t)m;
        else
            ratio = 0;                    /* future terms will be zero */

        if (m > 1) m -= 2;
        else m = 0;                       /* avoid unsigned wrap */
    }

    return sum;
}
