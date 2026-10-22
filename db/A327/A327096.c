/*
 * A327096  Expansion of Sum_{k>=1} sigma(k) * x^k / (1 - x^(2*k)), where
 *          sigma = A000203.
 * https://oeis.org/A327096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n, (n/d) odd} sigma(d), where sigma(d) is the sum
 *            of all positive divisors of d (OEIS A000203). This implements
 *            the generating function Sum_{k>=1} sigma(k)*x^k/(1-x^(2k))
 * Range:     u8 n <= 71, u16 n <= 9123, u32 n <= 9123, u64 n <= 9123,
 *            u128 n <= 9123, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* helper: sum of divisors of n (sigma(n)) */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            sum += (seq_t)i;
            idx_t j = n / i;
            if (j != i)
                sum += (seq_t)j;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for n<1, return 0 */

    seq_t result = (seq_t)0;

    /* enumerate divisors d of n up to sqrt(n) */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;   /* paired divisor */

            /* for divisor d, the corresponding quotient is e */
            if (e & 1U)            /* e odd  -> include sigma(d) */
                result += sigma_of(d);

            if (e != d && (d & 1U))   /* for paired divisor e, check d odd */
                result += sigma_of(e);
        }
    }

    return result;
}
