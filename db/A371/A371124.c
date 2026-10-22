/*
 * A371124  a(n) is the least nonnegative integer y such that y^2 = x^2 - k*n
 *          for k and x where n > k >= 1 and n > x >= floor(sqrt(n)).
 * https://oeis.org/A371124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor sqrt(A306284(n)^2 - n*A138191(n))
 * Range:     u8 n <= 33, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed directly by finding, for each n, the minimal k and corresponding x
   such that y^2 = x^2 - k*n.  The smallest y over the smallest k is returned. */

#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, but return 0 */
        return 0;

    seq_t sqrt_n = floor_sqrt((seq_t)n);

    for (idx_t k = 1; k <= n; ++k)            /* try increasing k (k <= n) */
    {
        seq_t m = (seq_t)k * n;                /* m = k * n */
        idx_t sqrt_m = (idx_t)floor_sqrt(m);  /* integer sqrt of m */
        seq_t best_y = (seq_t)(-1);           /* sentinel for no y yet */

        for (idx_t u = 1; u <= sqrt_m; ++u)   /* iterate divisors u of m */
        {
            if (m % (seq_t)u != 0)
                continue;

            seq_t v = m / (seq_t)u;           /* v = m / u  */
            if ((u + v) & 1)                  /* need same parity  */
                continue;

            seq_t x = (u + v) >> 1;           /* x = (u+v)/2       */
            if (x < sqrt_n || x > n)          /* bounds on x        */
                continue;

            seq_t y = (v - u) >> 1;           /* y = (v-u)/2       */
            if (best_y == (seq_t)(-1) || y < best_y)
                best_y = y;

            if (best_y == 0)                  /* cannot get better  */
                break;
        }

        if (best_y != (seq_t)(-1))            /* solution found     */
            return best_y;
    }

    return 0; /* unreachable for valid n */
}
