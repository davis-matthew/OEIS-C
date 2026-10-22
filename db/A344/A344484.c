/*
 * A344484  a(n) = Sum_{d|n} d^phi(n/d).
 * https://oeis.org/A344484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} d^φ(n/d)
 * Range:     u8 n <= 19, u16 n <= 33, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* iterate over all divisors using the standard O(sqrt(n)) method */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor d = i,  m = n/d */
            seq_t d = (seq_t)i;
            seq_t m = (seq_t)n / d;
            seq_t phi_val = phi((idx_t)m);
            seq_t term = big_pow(d, phi_val);
            sum += term;

            /* if this is not the square root, process the paired divisor */
            if (i != m) {
                d = (seq_t)m;          /* d = n/i */
                m = (seq_t)n / d;      /* m = i */
                phi_val = phi((idx_t)m);
                term = big_pow(d, phi_val);
                sum += term;
            }
        }
    }
    return sum;
}
