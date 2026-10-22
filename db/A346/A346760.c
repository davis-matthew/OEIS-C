/*
 * A346760  a(n) = Sum_{d|n} mu(n/d) * binomial(d,3).
 * https://oeis.org/A346760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * binomial(d, 3)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   binomial, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For n < 3 all binomial(d,3) are zero, hence a(n) = 0. */
    if (n < 3) return (seq_t)0;

    seq_t plus  = 0;   /* Sum of terms with mu = +1 */
    seq_t minus = 0;   /* Sum of terms with mu = -1 */

    for (idx_t i = (idx_t)1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* First divisor d = i */
            idx_t d    = i;
            seq_t bin  = binomial(d, (idx_t)3);
            int mu     = (int)mobius(n / d);
            if (mu == 1) plus += bin;
            else if (mu == -1) minus += bin;

            /* Symmetric divisor d = n / i, if distinct */
            if (i != n / i) {
                d    = n / i;
                bin  = binomial(d, (idx_t)3);
                mu   = (int)mobius(n / d);
                if (mu == 1) plus += bin;
                else if (mu == -1) minus += bin;
            }
        }
    }

    /* The true result is non‑negative; guard against underflow just in case. */
    return plus >= minus ? plus - minus : minus - plus;
}
