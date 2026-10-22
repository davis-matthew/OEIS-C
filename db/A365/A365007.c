/*
 * A365007  a(n) = Sum_{d|n} (-1)^(n/d-1) * binomial(d+1,2).
 * https://oeis.org/A365007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Σ_{d|n} (-1)^(n/d-1) * binomial(d+1,2) with
 *            binomial(d+1,2) = d(d+1)/2
 * Range:     u8 n <= 20, u16 n <= 344, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Compute a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety for n==0, though offset is 1 */

    /* Integer floor of sqrt(n).  Use double sqrt for speed, then correct. */
    idx_t sq = (idx_t)sqrt((long double)n);
    /* Correct any possible rounding error */
    while ((sq + 1) <= n / (sq + 1)) ++sq;
    while (sq > n / sq) --sq;

    int64_t sum = 0;  /* signed accumulator */

    for (idx_t d = 1; d <= sq; ++d) {
        if (n % d == 0) {
            idx_t nd = n / d;   /* the paired divisor */

            /* contribution of divisor d */
            int64_t coeff = (int64_t)d * (d + 1) / 2;
            if (nd & 1)       /* n/d is odd → + */
                sum += coeff;
            else
                sum -= coeff;

            /* contribution of the paired divisor nd when distinct */
            if (d != nd) {
                int64_t coeff2 = (int64_t)nd * (nd + 1) / 2;
                if (d & 1)   /* n/(nd) == d, so parity of d decides sign */
                    sum += coeff2;
                else
                    sum -= coeff2;
            }
        }
    }

    return (seq_t)sum;
}
