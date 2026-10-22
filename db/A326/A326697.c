/*
 * A326697  a(n) is the sum of divisors d of n such that sigma(d) divides n.
 * https://oeis.org/A326697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, σ(d) | n} d (sum of divisors d of n such that
 *            σ(d) divides n)
 * Range:     u8 n <= 359, u16 n <= 8250, u32 n <= 8250, u64 n <= 8250,
 *            u128 n <= 8250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute σ(m), the sum of all positive divisors of m. */
static inline seq_t sigma_of(idx_t m)
{
    seq_t s = 0;
    for (idx_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            s += (seq_t)i;
            idx_t j = m / i;
            if (j != i) s += (seq_t)j;
        }
    }
    return s;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* first divisor d */
            seq_t sigma_d = sigma_of(d);
            if ((seq_t)n % sigma_d == 0)
                result += (seq_t)d;

            /* second divisor e = n/d, if distinct from d */
            idx_t e = n / d;
            if (e != d) {
                seq_t sigma_e = sigma_of(e);
                if ((seq_t)n % sigma_e == 0)
                    result += (seq_t)e;
            }
        }
    }
    return result;
}
