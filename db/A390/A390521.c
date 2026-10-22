/*
 * A390521  a(n) = limiting prime of the recurrence x(k) = 1 + (phi(x(k-1)) +
 *          phi(x(k-2))) / 2 started at x(0) = x(1) = n.
 * https://oeis.org/A390521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = limiting prime of the recurrence * x(k) = 1 +
 *            (phi(x(k-1)) + phi(x(k-2))) / 2 * with initial values x(0) =
 *            x(1) = n * This function returns the converged prime a(n)
 * Range:     u8 n <= 248, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    if (n == 0)         /* undefined for indices < 1, safety */
        return 0;

    seq_t x0 = (seq_t)n;   /* x(0) */
    seq_t x1 = x0;         /* x(1) */

    while (1) {
        seq_t phi0 = calc_phi(x0);
        seq_t phi1 = calc_phi(x1);
        seq_t sum  = phi0 + phi1;   /* sum fits in seq_t */
        seq_t x2   = 1 + sum / 2;   /* integer division */

        if (x2 == x1 && x1 == x0)   /* reached fixed point (prime) */
            return x2;

        x0 = x1;
        x1 = x2;
    }
}
