/*
 * A319712  Sum of A034968(d) over divisors d of n, where A034968 gives the
 *          sum of digits in factorial base.
 * https://oeis.org/A319712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{d|n} A034968(d), where A034968(d) is the sum of
 *            digits of d in factorial base
 * Range:     u8 n <= 1423, u16 n <= 40320, u32 n <= 40320, u64 n <= 40320,
 *            u128 n <= 40320, big any n
 * Validated: standard, pass, 111 terms through n = 9494 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute the sum of digits of n in factorial base. */
static seq_t factorial_base_sum(idx_t n)
{
    seq_t sum = 0;
    seq_t temp = (seq_t)n;
    idx_t div = 2;                /* divisor starts at 2 = (1+1) */

    while (temp != 0) {
        sum += temp % (seq_t)div;
        temp /= (seq_t)div;
        ++div;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            res += factorial_base_sum(d);
            /* paired divisor n/d if it is distinct */
            idx_t d2 = n / d;
            if (d2 != d)
                res += factorial_base_sum(d2);
        }
    }
    return res;
}
