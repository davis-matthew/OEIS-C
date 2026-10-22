/*
 * A332467  a(n) = n! * Sum_{d|n} mu(d) / (d!)^(n/d).
 * https://oeis.org/A332467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{d|n} mu(d) / (d!)^(n/d)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   big_pow, factorial, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/big_pow.h"
#include "../utils/mu_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */

    seq_t fac_n = factorial(n);
    seq_t sum = 0;

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0) continue;

        int mu_d = mu_of(d);
        if (mu_d != 0)
        {
            seq_t term = fac_n / big_pow(factorial(d), (seq_t)(n / d));
            if (mu_d > 0)
                sum += term;
            else
                sum -= term;
        }

        idx_t e = n / d;
        if (e == d) continue;     /* handle square‑root divisor once */

        int mu_e = mu_of(e);
        if (mu_e != 0)
        {
            seq_t term = fac_n / big_pow(factorial(e), (seq_t)(n / e));
            if (mu_e > 0)
                sum += term;
            else
                sum -= term;
        }
    }

    return sum;
}
