/*
 * A318878  Sum of A083254(d) for all such divisors d of n for which
 *          A083254(d) > 0.
 * https://oeis.org/A318878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, 2*φ(d) - d > 0} (2*φ(d) - d), where φ = Euler's
 *            totient
 * Range:     u8 n <= 256, u16 n <= 9772, u32 n <= 9772, u64 n <= 9772,
 *            u128 n <= 9772, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/phi.h"

/* Return f(d) = 2*φ(d) - d if this value is positive, otherwise 0.
   The computation avoids unsigned underflow by using the equivalent
   expression φ(d) - (d - φ(d)), which is positive iff 2*φ(d) > d. */
static seq_t positive_f(idx_t d)
{
    seq_t phi_val = phi(d);
    /* Calculate d - φ(d); this is safe because φ(d) ≤ d. */
    seq_t diff = (seq_t)d - phi_val;
    /* f(d) > 0  ⇔  φ(d) > floor(d/2). */
    if (phi_val > (seq_t)(d / 2))
    {
        /* φ(d) - (d - φ(d)) = 2*φ(d) - d, which is safe from overflow. */
        return phi_val - diff;
    }
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for index 0, safety guard */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            sum += positive_f(i);
            idx_t j = n / i;
            if (j != i)
                sum += positive_f(j);
        }
    }
    return sum;
}
