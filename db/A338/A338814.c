/*
 * A338814  Expansion of e.g.f. log(Product_{k>0} (1 + x^k)^(1/k)).
 * https://oeis.org/A338814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n-1)! * Σ_{d|n} (-1)^{d+1}
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 14, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = (n-1)! * A048272(n) */

#include <stddef.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Handle the offset: n >= 1 */
    if (n == 0)
        return 0;               /* Undefined for n=0, return 0 as safe value */

    /* Compute Σ_{d|n} (-1)^{d+1} */
    seq_t s = 0;
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            s += (seq_t)((d & 1) ? 1 : -1);

            idx_t e = n / d;
            if (e != d)
                s += (seq_t)((e & 1) ? 1 : -1);
        }
    }

    /* Compute (n-1)! using the provided helper */
    seq_t fact = factorial(n - 1);

    /* Return the product */
    return fact * s;
}
