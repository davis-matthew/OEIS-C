/*
 * A348304  a(n) = Sum_{d|n} d'', where d'' is the second arithmetic
 *          derivative of d (A068346).
 * https://oeis.org/A348304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{d|n} d'' where d'' is the second arithmetic
 *            derivative of d The code factors each divisor d, computes its
 *            first derivative d' by d' = Σ (n/p_i) * e_i for the prime
 *            factorization d = ∏ p_i^{e_i}, then computes the second
 *            derivative d'' = (d')' by the same routine. Finally it sums d''
 *            o...
 * Range:     u8 n <= 31, u16 n <= 1535, u32 n <= 9883, u64 n <= 9883,
 *            u128 n <= 9883, big any n
 * Helpers:   first_derivative_u64, second_derivative_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/first_derivative_u64.h"
#include "../utils/second_derivative_u64.h"

/*--- Main sequence function -----------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1)   /* OEIS offset is 1; behaviour for n=0 is irrelevant */
        return 0;

    seq_t total = 0;
    /* Enumerate all divisors d of n */
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            total += second_derivative_u64(d);
            idx_t e = n / d;
            if (e != d)          /* avoid double‑counting perfect square root */
                total += second_derivative_u64(e);
        }
    }
    return total;
}
