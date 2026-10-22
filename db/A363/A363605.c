/*
 * A363605  Expansion of Sum_{k>0} x^(2*k)/(1-x^k)^5.
 * https://oeis.org/A363605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 556, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom4_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{d|n} binomial(d+2, 4)
 *            (equivalently, a(n) = (σ₄(n) + 2σ₃(n) - σ₂(n) - 2σ₁(n)) / 24)
 */
#include <stdint.h>  /* for uint64_t */
#include "../utils/binom4_divisor.h"

seq_t a(idx_t n)
{
    /* n >= 1; behaviour for n < 1 is irrelevant as per requirements */
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            sum += binom4_divisor(d);
            idx_t other = n / d;
            if (other != d)
                sum += binom4_divisor(other);
        }
    }
    return sum;
}
