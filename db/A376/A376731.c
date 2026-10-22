/*
 * A376731  Expansion of (1 - x^4 - x^5)/((1 - x^4 - x^5)^2 - 4*x^9).
 * https://oeis.org/A376731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} binomial(2*k, 2*n-8*k)
 * Range:     u8 n <= 17, u16 n <= 35, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* See OEIS A376731 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 4;
    idx_t double_n = 2 * n;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* Compute 8*k carefully to avoid underflow */
        if (8 * k > double_n)
            continue;                     /* 2*n-8*k would be negative */

        idx_t top    = 2 * k;              /* 2*k   */
        idx_t bottom = double_n - 8 * k;   /* 2*n-8*k */

        if (bottom <= top)
            sum += binomial(top, bottom);
    }
    return sum;
}
