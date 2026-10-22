/*
 * A306260  Number of ways to write n as w*(4w+1) + x*(4x-1) + y*(4y-2) +
 *          z*(4z-3) with w,x,y,z nonnegative integers.
 * https://oeis.org/A306260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of non‑negative integer solutions (w,x,y,z) of n =
 *            w*(4w+1)+x*(4x-1)+y*(4y-2)+z*(4z-3)
 * Range:     u8 n <= 3719, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is intentionally simple:  nested loops are
   bounded by the size needed to reach n, so for n≤10000 the
   routine runs in a few milliseconds.  All intermediate
   arithmetic is performed in idx_t (uint64_t) to avoid
   overflow even when seq_t is an 8‑bit type. */

#include <stdint.h>   /* for uint64_t used in idx_t definitions */

seq_t a(idx_t n)
{
    /* Cast n to idx_t (already is) for local use */
    idx_t sn = n;
    idx_t count = 0;

    for (idx_t w = 0; ; ++w) {
        /* w_term = w*(4*w+1)  */
        idx_t w_term = w * (4 * w + 1);
        if (w_term > sn) break;

        for (idx_t x = 0; ; ++x) {
            /* x_term = x*(4*x-1)  */
            idx_t x_term = x * (4 * x - 1);
            idx_t wx = w_term + x_term;
            if (wx > sn) break;

            for (idx_t y = 0; ; ++y) {
                /* y_term = y*(4*y-2)  */
                idx_t y_term = y * (4 * y - 2);
                idx_t wxy = wx + y_term;
                if (wxy > sn) break;

                for (idx_t z = 0; ; ++z) {
                    /* z_term = z*(4*z-3)  */
                    idx_t z_term = z * (4 * z - 3);
                    idx_t total = wxy + z_term;
                    if (total > sn) break;
                    if (total == sn) ++count;
                }
            }
        }
    }

    return (seq_t)count;
}
