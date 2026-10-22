/*
 * A332824  a(n) = Product_{d|n} A019565(phi(d)), where phi is Euler totient
 *          function A000010.
 * https://oeis.org/A332824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} A019565(phi(d))
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 71, u64 n <= 251,
 *            u128 n <= 791, big any n
 * Helpers:   A019565_value, calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Must be compiled for any integer type seq_t/idx_t as specified in the build. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/calc_phi.h"   /* for calc_phi(seq_t) */
#include "../utils/A019565_value.h"

/* Main function computing the sequence A332824. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset – harmless */

    seq_t prod = (seq_t)1;

    for (idx_t d = 1; d*d <= n; ++d) {
        if (n % d == 0) {
            idx_t other = n / d;

            /* process divisor d */
            idx_t phi_d = (idx_t)calc_phi((seq_t)d);
            prod *= A019565_value(phi_d);

            /* process complementary divisor other if distinct */
            if (other != d) {
                idx_t phi_o = (idx_t)calc_phi((seq_t)other);
                prod *= A019565_value(phi_o);
            }
        }
    }

    return prod;
}
