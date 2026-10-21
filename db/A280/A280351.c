/*
 * A280351  Expansion of Sum_{k>=0} (x/(1 - x))^(k^3).
 * https://oeis.org/A280351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..floor(n^(1/3))} binomial(n-1, k^3-1) (a(0) = 1)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 37, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binomial, int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t kmax = int_cube_root(n);
    seq_t sum = 0;

    for (idx_t k = 1; k <= kmax; ++k)
    {
        idx_t k3 = k * k * k;          /* k^3, fits in idx_t (k <= 21 for n <= 10000) */
        idx_t r  = k3 - 1;              /* r = k^3 - 1 <= n-1 */
        sum += binomial(n - 1, r);
    }

    return sum;
}
