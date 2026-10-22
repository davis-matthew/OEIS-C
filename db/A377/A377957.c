/*
 * A377957  The number of iterations of the map x -> x + A001065(x) if
 *          A009194(x) = 1, x -> x / A009194(x) otherwise, starting from n,
 *          until reaching 1.
 * https://oeis.org/A377957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the iterations until reaching 1 by repeatedly
 *            applying: if gcd(x, σ(x)) = 1 then x ← x + (σ(x) – x), otherwise
 *            x ← x / gcd(x, σ(x)). Here σ(x) is the sum of all divisors of x
 * Range:     u8 n <= 15, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* current value */
    seq_t steps = 0;            /* number of iterations */

    while (x != 1) {
        /* compute σ(x) = sum of divisors of x */
        seq_t sigma = 0;
        idx_t lim = (idx_t)floor_sqrt(x);   /* floor sqrt of x */

        for (idx_t d = 1; d <= lim; ++d) {
            if (x % (seq_t)d == 0) {
                sigma += d;
                seq_t other = x / (seq_t)d;
                if (other != (seq_t)d) {
                    sigma += other;
                }
            }
        }

        seq_t g = gcd(x, sigma);          /* gcd(x, σ(x)) */

        if (g == 1) {
            /* addition step: add proper divisors */
            x = x + (sigma - x);
        } else {
            /* division step */
            x = x / g;
        }

        ++steps;
    }

    return steps;
}
