/*
 * A394391  a(n) = tau(n)^2 + (n - tau(n))^2.
 * https://oeis.org/A394391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tau(n)^2 + (n - tau(n))^2 where tau(n) is the number of
 *            positive divisors of n
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return a(n) for index n (1‑based). */
seq_t a(idx_t n)
{
    /* Compute tau(n) by iterating over divisors up to sqrt(n). */
    seq_t tau = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* d is a divisor. Add 1 for d and another for n/d unless they coincide. */
            if (d * d == n)
                ++tau;
            else
                tau += 2;
        }
    }

    /* Compute a(n) = tau^2 + (n - tau)^2. */
    seq_t n_minus_tau = (seq_t)n - tau;
    return tau * tau + n_minus_tau * n_minus_tau;
}
