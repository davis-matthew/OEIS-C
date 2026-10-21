/*
 * A275675  Number of ordered ways to write n as 4^k*(1+x^2+y^2)+5*z^2, where
 *          k,x,y,z are nonnegative integers with x <= y.
 * https://oeis.org/A275675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 293, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count quadruples (k,x,y,z) of non‑negative integers with x <= y that satisfy
 * n = 4^k * (1 + x^2 + y^2) + 5 * z^2.
 * The function returns the number of such unordered 4‑tuples for the given n.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* offset starts at 1, but guard against n==0 */

    seq_t count = 0;
    idx_t base  = 1;          /* 4^k */

    /* Iterate over powers of 4 not exceeding n */
    for (idx_t k = 0; base <= n; ++k, base *= 4) {
        idx_t max_val = n / base;   /* maximum allowed value of (1 + x^2 + y^2) */

        /* Choose x with 1 + x^2 <= max_val */
        for (idx_t x = 0; 1 + x*x <= max_val; ++x) {
            idx_t xx = x*x;

            /* y runs from x upward; condition ensures 1 + x^2 + y^2 <= max_val */
            for (idx_t y = x; 1 + xx + y*y <= max_val; ++y) {
                idx_t val  = 1 + xx + y*y;        /* 1 + x^2 + y^2          */
                idx_t prod = base * val;          /* 4^k * (1+x^2+y^2)     */
                idx_t t    = n - prod;            /* remaining part for 5*z^2*/

                if ((t % 5) == 0) {
                    idx_t z2 = t / 5;            /* candidate z^2           */
                    seq_t s  = floor_sqrt((seq_t)z2);
                    if (s * s == z2)             /* perfect square test    */
                        ++count;
                }
            }
        }
    }
    return count;
}
