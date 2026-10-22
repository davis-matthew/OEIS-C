/*
 * A395990  a(n) = Sum_{d|n} d^(tau(n/d) mod 2).
 * https://oeis.org/A395990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} d^(tau(n/d) mod 2) (equivalently: for each
 *            divisor d of n, add d if n/d is a square, else add 1)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   is_square_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_square_u64.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 for the OEIS offset; for n == 0 just return 0 (unused). */
    if (n == 0)
        return (seq_t)0;

    seq_t sum = (seq_t)0;

    /* iterate over divisor pairs (d, n/d) */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        idx_t m = n / d;                /* n/d for the first divisor d   */
        if (is_square_u64(m))
            sum += (seq_t)d;
        else
            sum += (seq_t)1;

        idx_t d2 = n / d;               /* the paired divisor n/d       */
        if (d2 != d) {                  /* skip the square root case     */
            idx_t m2 = n / d2;          /* which is equal to d           */
            if (is_square_u64(m2))
                sum += (seq_t)d2;
            else
                sum += (seq_t)1;
        }
    }

    return sum;
}
