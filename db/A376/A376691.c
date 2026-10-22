/*
 * A376691  The number of solutions to x + y == x^2 + y^2 == x^3 + y^3 (mod n)
 *          with 0 <= x <= y < n.
 * https://oeis.org/A376691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count of solutions to x + y == x^2 + y^2 == x^3 + y^3 (mod n)
 *            with 0 <= x <= y < n
 * Range:     u8 n <= 11, u16 n <= 54, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   count_solutions_for_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_solutions_for_sum.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t mod = (seq_t)n;
    for (idx_t t = 0; t < n; ++t)
    {
        idx_t t1 = t;
        idx_t t2 = (t + n - 1) % n;   /* (t-1) mod n */
        idx_t t3 = (t + n - 2) % n;   /* (t-2) mod n */

        seq_t prod = (seq_t)t1 * (seq_t)t2 * (seq_t)t3;
        if (prod % mod != 0)
            continue; /* not a root of t(t-1)(t-2) mod n */

        /* First candidate sum s = t */
        result += count_solutions_for_sum(n, t);

        /* Second candidate sum s = t + n, if within the allowed range */
        if (n > 1 && t <= n - 2)
            result += count_solutions_for_sum(n, t + n);
    }
    return result;
}
