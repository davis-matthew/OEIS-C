/*
 * A327249  Expansion of Sum_{k>=1} x^k * (1 + k * x^k)^k.
 * https://oeis.org/A327249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} (n/d)^(d-1) * binomial(n/d, d-1)
 * Range:     u8 n <= 15, u16 n <= 34, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/binomial.h"

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t e = n / d; /* e = n/d */

            /* term for divisor d: (n/d)^(d-1) * binomial(n/d, d-1) */
            seq_t base  = (seq_t)e;
            seq_t term  = seq_pow(base, d - 1);
            term = term * binomial(e, d - 1);
            result += term;

            /* term for divisor e, when distinct from d */
            if (d != e) {
                seq_t base2 = (seq_t)d;
                seq_t term2 = seq_pow(base2, e - 1);
                term2 = term2 * binomial(d, e - 1);
                result += term2;
            }
        }
    }
    return result;
}
