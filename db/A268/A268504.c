/*
 * A268504  a(n) = Product_{k=0..n} (3*k)!.
 * https://oeis.org/A268504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0..n} (3*k)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 1;      /* Final product */
    seq_t fac = 1;      /* Current factorial value (3*k)! */
    idx_t prev_m = 0;   /* Previously processed 3*k */

    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t m = 3 * k;            /* Target factorial argument */
        if (m > prev_m)
        {
            /* Multiply fac by each integer from prev_m+1 to m */
            for (idx_t x = prev_m + 1; x <= m; ++x)
                fac *= (seq_t)x;
            prev_m = m;
        }
        res *= fac;                 /* Multiply into the running product */
    }

    return res;
}
