/*
 * A342473  a(n) = Sum_{d|n} phi(d)^d.
 * https://oeis.org/A342473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d)^d
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/phi.h"
#include "../utils/big_pow.h"

/* Computes a(n) = Σ_{d|n} phi(d)^d for any n >= 1 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor d1 = i */
            seq_t phi_d1 = phi(i);
            seq_t term_d1 = big_pow(phi_d1, i);
            sum += term_d1;

            /* divisor d2 = n/i, if distinct */
            idx_t d2 = n / i;
            if (d2 != i) {
                seq_t phi_d2 = phi(d2);
                seq_t term_d2 = big_pow(phi_d2, d2);
                sum += term_d2;
            }
        }
    }
    return sum;
}
