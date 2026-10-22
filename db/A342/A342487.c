/*
 * A342487  a(n) = Sum_{d|n} phi(d)^(n+1).
 * https://oeis.org/A342487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d)^(n+1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   phi, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/seq_pow.h"
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n=0, protect */
    seq_t sum = 0;
    idx_t exp = n + 1;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            seq_t phi_d = phi(d);
            seq_t pow_d = seq_pow(phi_d, exp);
            sum += pow_d;
            if (d != n / d) { /* second divisor */
                seq_t phi_nd = phi(n / d);
                seq_pow( phi_nd, exp ); /* unused: correct sequence? */
                seq_t pow_nd = seq_pow(phi_nd, exp);
                sum += pow_nd;
            }
        }
    }
    return sum;
}
